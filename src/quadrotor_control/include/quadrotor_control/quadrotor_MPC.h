#ifndef QUADROTOR_MPC_INCLUDE
#define QUADROTOR_MPC_INCLUDE
#include <casadi/casadi.hpp>
#include <Eigen/Dense>
#include <chrono>

// Define the MPCC class
struct Point
{
    double x, y, z;
};
class MPC
{
public:
    // Constructor
    MPC(){}; 
    /**
 * MPC controller for quadrotors.
 *
 * @param mass  mass.
 * @param Ix inertia around x axis.
 * @param Iy inertia around y axis.
 * @param Iz inertia around z axis.
 * @param wrench2rotorthrust trasnforms wrench control commands to individual rotor thrusts (written as vector).
 * @param thrust2wieght thrust to wieght ratio
 * @param dt time step
 * @param n number of nodes
 */
    MPC(double mass, double Ix, double Iy, double Iz, casadi::DM wrench2rotorthrust, double thrust2wieght, double dt, int n)
    {
        opti.solver("ipopt", p_opts);
        N = n;
        init_u = casadi::DM::zeros(4, N);
        init_p = casadi::DM::zeros(3, N + 1);
        init_v = casadi::DM::zeros(3, N + 1);
        init_omega = casadi::DM::zeros(3, N + 1);
        pos_ = opti.variable(3, N + 1); // State variables
        vel_ = opti.variable(3, N + 1);
        omega_ = opti.variable(3, N + 1);
        u_ = opti.variable(4, N);
        p_i = opti.parameter(3);
        vel_i = opti.parameter(3);
        omega_i = opti.parameter(3);
        p_d = opti.parameter(3, N + 1);
        v_d = opti.parameter(3, N + 1);
        wz_d = opti.parameter(N + 1);
        Q_cost = opti.parameter(12);
        m = mass;
        init_u(3, all) = m * g;
        I = casadi::DM::diag(casadi::DM::vertcat({Ix, Iy, Iz}));
        t2w = thrust2wieght;
        dt_ = dt;
        wrench2thrust = casadi::DM::reshape(wrench2rotorthrust, 4, 4);

        init_q = casadi::DM::zeros(4, N + 1);
        init_q(0, all) = 1;
        q_ = opti.variable(4, N + 1);
        q_i = opti.parameter(4);
        q_f = opti.parameter(4, N + 1);
        cost = costFunction_q(p_d, v_d, q_f, wz_d, pos_, vel_, q_, omega_, u_);
        opti.minimize(cost);
        opti.subject_to(pos_(all, 0) == p_i);
        opti.subject_to(vel_(all, 0) == vel_i);
        opti.subject_to(q_(all, 0) == q_i);
        opti.subject_to(omega_(all, 0) == omega_i);
        opti.set_initial(u_, init_u);
        opti.set_initial(pos_, init_p);
        opti.set_initial(vel_, init_v);
        opti.set_initial(q_, init_q);
        opti.set_initial(omega_, init_omega);
        for (int i = 0; i < N; i++)
        {
            auto [p_next, v_next, q_next, omega_next] = integrate_q(pos_(all, i), vel_(all, i), q_(all, i), omega_(all, i), u_(all, i));
            casadi::MX rotors_thrust = casadi::MX::mtimes(wrench2thrust, u_(all, i));
            double max_thrust = m * g * t2w / 4;

            opti.subject_to(rotors_thrust(0) > 0);
            opti.subject_to(rotors_thrust(1) > 0);
            opti.subject_to(rotors_thrust(2) > 0);
            opti.subject_to(rotors_thrust(3) > 0);
            opti.subject_to(rotors_thrust(0) < max_thrust);
            opti.subject_to(rotors_thrust(1) < max_thrust);
            opti.subject_to(rotors_thrust(2) < max_thrust);
            opti.subject_to(rotors_thrust(3) < max_thrust);

            opti.subject_to(pos_(all, i + 1) - p_next == 0);

            opti.subject_to(vel_(all, i + 1) - v_next == 0);

            opti.subject_to(q_(all, i + 1) - q_next == 0);

            opti.subject_to(omega_(all, i + 1) - omega_next == 0);
        }
    };
    /**
 * Solve MPC.
 *
 * @param p_0  initial position.
 * @param v_0 initial velocity.
 * @param q0 inertia orientation (as quatenion).
 * @param omega_0 initial body rates.
 * @param p_g desired future positions.
 * @param v_g desired future velocities.
 * @param q_g desired future orientations.
 * @param wz_g desired future body rates.
 * @param Q weights vector.
 * @return optimal control commands (Wrench).
 */
    std::vector<double> solve(const std::vector<double> &p_0, const std::vector<double> &v_0, const std::vector<double> &q_0, const std::vector<double> &omega_0, const std::vector<Point> &p_g, const std::vector<Point> &v_g, const std::vector<Eigen::Quaterniond> &q_g, const std::vector<double> &wz_g, const std::vector<double> &Q)
    {
        // Define the optimization variables
        // Set the initial state

        casadi::DM p_g_c = casadi::DM::zeros(3, N + 1);
        casadi::DM v_g_c = casadi::DM::zeros(3, N + 1);
        casadi::DM q_g_c = casadi::DM::zeros(4, N + 1);
        casadi::DM wz_g_c = casadi::DM::zeros(N + 1);

        for (int i = 0; i <= N; i++)
        {
            p_g_c(0, i) = p_g[i].x;
            p_g_c(1, i) = p_g[i].y;
            p_g_c(2, i) = p_g[i].z;
        }
        for (int i = 0; i <= N; i++)
        {
            v_g_c(0, i) = v_g[i].x;
            v_g_c(1, i) = v_g[i].y;
            v_g_c(2, i) = v_g[i].z;
        }
        for (int i = 0; i <= N; i++)
        {
            q_g_c(0, i) = q_g[i].w();
            q_g_c(1, i) = q_g[i].x();
            q_g_c(2, i) = q_g[i].y();
            q_g_c(3, i) = q_g[i].z();
        }

        opti.set_value(p_i, p_0);
        opti.set_value(p_d, p_g_c);
        opti.set_value(v_d, v_g_c);
        opti.set_value(q_i, q_0);
        opti.set_value(q_f, q_g_c);
        opti.set_value(vel_i, v_0);
        opti.set_value(omega_i, omega_0);
        opti.set_value(wz_d, wz_g);
        opti.set_value(Q_cost, Q);
        opti.set_initial(pos_, init_p);
        opti.set_initial(vel_, init_v);
        opti.set_initial(q_, init_q);
        opti.set_initial(omega_, init_omega);
        opti.set_initial(u_, init_u);
        auto r = opti.solve_limited();

        auto x_opt = r.value(pos_);
        auto v_opt = r.value(vel_);
        auto omega_opt = r.value(omega_);
        init_p = r.value(pos_); 
        init_v = r.value(vel_); 
        init_u = r.value(u_);
        init_q = r.value(q_);
        init_omega = r.value(omega_);

        std::vector<double> u_opt;
        u_opt.push_back(double(r.value(u_(0, 0))));
        u_opt.push_back(double(r.value(u_(1, 0))));
        u_opt.push_back(double(r.value(u_(2, 0))));
        u_opt.push_back(double(r.value(u_(3, 0))));

        return u_opt;
    }

private:
    std::tuple<casadi::MX, casadi::MX, casadi::MX> dynamics_q(const casadi::MX &pos, const casadi::MX &vel, const casadi::MX &q, const casadi::MX &omega, const casadi::MX &u)
    {
        casadi::MX current_pos=pos;
        casadi::MX F_x = 2 * (q(0) * q(2) + q(1) * q(3)) * u(3);
        casadi::MX F_y = 2 * (q(2) * q(3) - q(0) * q(1)) * u(3);
        casadi::MX F_z = (1 - 2 * q(1) * q(1) - 2 * q(2) * q(2)) * u(3) - m * g;
        casadi::MX F = casadi::MX::vertcat({F_x, F_y, F_z});
        casadi::DM I_inv = casadi::DM::diag(casadi::DM::vertcat({1 / I(0, 0), 1 / I(1, 1), 1 / I(2, 2)}));
        casadi::MX L_dot = casadi::MX::vertcat({u(0), u(1), u(2)}) - casadi::MX::mtimes(skew(omega), casadi::MX::vertcat({I(0, 0) * omega(0), I(1, 1) * omega(1), I(2, 2) * omega(2)}));
        casadi::MX pos_dot = vel;
        casadi::MX vel_dot = F / m;
        casadi::MX omega_dot = casadi::MX::vertcat({I_inv(0, 0) * L_dot(0), I_inv(1, 1) * L_dot(1), I_inv(2, 2) * L_dot(2)});
        auto out = std::make_tuple(pos_dot, vel_dot, omega_dot);

        return out;
    }

    std::tuple<casadi::MX, casadi::MX, casadi::MX, casadi::MX> integrate_q(const casadi::MX &pos, const casadi::MX &vel, const casadi::MX &q, const casadi::MX &omega, const casadi::MX &u)
    {

        auto [k1_pos, k1_vel, k1_omega] = dynamics_q(pos, vel, q, omega, u);
        auto k1_q = omega;
        auto q_2 = quat_integrate(q, k1_q, dt_ / 2.0);
        auto [k2_pos, k2_vel, k2_omega] = dynamics_q(pos + dt_ / 2.0 * k1_pos, vel + dt_ / 2.0 * k1_vel, q_2, omega + dt_ / 2.0 * k1_omega, u);
        auto k2_q = omega + dt_ / 2.0 * k1_omega;
        auto q_3 = quat_integrate(q, k2_q, dt_ / 2.0);
        auto [k3_pos, k3_vel, k3_omega] = dynamics_q(pos + dt_ / 2.0 * k2_pos, vel + dt_ / 2.0 * k2_vel, q_3, omega + dt_ / 2.0 * k2_omega, u);
        auto k3_q = omega + dt_ / 2.0 * k2_omega;
        auto q_4 = quat_integrate(q, k3_q, dt_);
        auto [k4_pos, k4_vel, k4_omega] = dynamics_q(pos + dt_ * k3_pos, vel + dt_ * k3_vel, q_4, omega + dt_ * k3_omega, u);
        auto k4_q = omega + dt_ * k3_omega;
        auto position_next = pos + dt_ / 6.0 * (k1_pos + 2.0 * k2_pos + 2.0 * k3_pos + k4_pos);
        auto velocity_next = vel + dt_ / 6.0 * (k1_vel + 2.0 * k2_vel + 2.0 * k3_vel + k4_vel);
        auto q_next = quat_integrate(q, (k1_q + 2.0 * k2_q + 2.0 * k3_q + k4_q) / 6.0, dt_); //
        auto omega_next = omega + dt_ / 6.0 * (k1_omega + 2.0 * k2_omega + 2.0 * k3_omega + k4_omega);
        // auto position_next = pos + dt_*k1_pos;
        // auto velocity_next=vel+dt_*k1_vel;
        // auto q_next = quat_integrate(q, k1_q, dt_);
        // auto omega_next = omega + dt_*k1_omega;
        return std::make_tuple(position_next, velocity_next, q_next, omega_next);
    }

    casadi::MX skew(const casadi::MX &v)
    {
        casadi::MX S = casadi::MX::zeros(3, 3);
        S(1, 0) = v(2);
        S(2, 0) = -v(1);
        S(0, 1) = -v(2);
        S(2, 1) = v(0);
        S(0, 2) = v(1);
        S(1, 2) = -v(0);
        return S;
    }
    // v function
    casadi::MX unskew(const casadi::MX &S)
    {
        casadi::MX v = casadi::MX::vertcat({S(2, 1), S(0, 2), S(1, 0)});
        return v;
    }

    // multiply quaternions
    casadi::MX quatmultiply(const casadi::MX &q1, const casadi::MX &q2)
    {
        casadi::MX q = casadi::MX::vertcat({q1(0) * q2(0) - q1(1) * q2(1) - q1(2) * q2(2) - q1(3) * q2(3),
                                            q1(0) * q2(1) + q1(1) * q2(0) + q1(2) * q2(3) - q1(3) * q2(2),
                                            q1(0) * q2(2) - q1(1) * q2(3) + q1(2) * q2(0) + q1(3) * q2(1),
                                            q1(0) * q2(3) + q1(1) * q2(2) - q1(2) * q2(1) + q1(3) * q2(0)});
        return q;
    }
    // integrate quaternions
    casadi::MX quat_integrate(const casadi::MX &q, const casadi::MX &omega, const double &dt)
    {

        casadi::MX omega_norm = sqrt((casadi::MX::dot(omega, omega) + 1e-6));
        casadi::MX dtheta = 0.5 * dt * omega_norm;
        casadi::MX axis = omega / (omega_norm);

        casadi::MX dq = casadi::MX::vertcat({cos(dtheta), sin(dtheta) * axis});

        casadi::MX q_next = quatmultiply(q, dq);

        return q_next;
    }

    // Define the cost function
    casadi::MX costFunction_q(const casadi::MX &pose_g, const casadi::MX &vel_g, const casadi::MX &q_g, const casadi::MX &wz_g, const casadi::MX &pos_, const casadi::MX &vel_, const casadi::MX &q_, const casadi::MX &omega_, const casadi::MX &u_)
    {
        // Define the cost function
        casadi::MX omega_g = casadi::MX::vertcat({0, 0, wz_g(0)});
        casadi::MX p_error = (pos_(all, 0) - pose_g(all, 0));
        casadi::MX v_error = (vel_(all, 0) - vel_g(all, 0));
        casadi::MX q_pred = q_(all, 0);
        casadi::MX q_g_cong = casadi::MX::vertcat({q_g(0, 0), -q_g(casadi::Slice(1, 4), 0)});
        casadi::MX q_error = quatmultiply(q_pred, q_g_cong);
        casadi::MX omega_error = (omega_(all, 0) - omega_g);
        casadi::DM R = casadi::DM::diag(casadi::DM::vertcat({0.0, 0.0, 0.0, 0.00}));
        casadi::MX Q = casadi::MX::diag(Q_cost);

        casadi::MX state_error = casadi::MX::vertcat({p_error, v_error, q_error(casadi::Slice(1, 4)), omega_error});
        casadi::MX cost = casadi::MX::mtimes(casadi::MX::mtimes(state_error.T(), Q), state_error);
        for (int i = 0; i < N; i++)
        {
            omega_g = casadi::MX::vertcat({0, 0, wz_g(i + 1)});
            p_error = (pos_(all, i + 1) - pose_g(all, i + 1));
            v_error = (vel_(all, i + 1) - vel_g(all, i + 1));
            q_pred = q_(all, i + 1);
            q_g_cong = casadi::MX::vertcat({q_g(0, i + 1), -q_g(casadi::Slice(1, 4), i + 1)});
            q_error = quatmultiply(q_pred, q_g_cong);
            omega_error = (omega_(all, i + 1) - omega_g);
            state_error = casadi::MX::vertcat({p_error, v_error, q_error(casadi::Slice(1, 4)), omega_error});

            cost += casadi::MX::mtimes(casadi::MX::mtimes(u_(all, i).T(), R), u_(all, i)) + casadi::MX::mtimes(casadi::MX::mtimes(state_error.T(), Q), state_error);
        }
        return cost;
    }

private:
    int N;
    casadi::Opti opti = casadi::Opti();
    casadi::Dict p_opts = {{"expand", true}, {"print_time", false}, {"ipopt.print_level", 0}, {"ipopt.acceptable_iter", 1}, {"ipopt.acceptable_tol", 1e-3}, {"ipopt.acceptable_obj_change_tol", 1e-3}};
    casadi::MX pos_; // State variables
    casadi::MX vel_;
    casadi::MX q_;
    casadi::MX omega_;
    casadi::MX u_; // Control variables

    casadi::MX p_d; // goal state
    casadi::MX v_d;
    casadi::MX q_f;
    casadi::MX wz_d;

    casadi::MX p_i; // initial state
    casadi::MX vel_i;
    casadi::MX q_i;
    casadi::MX Q_cost;

    casadi::MX omega_i;
    casadi::MX cost; // cost func

    double dt_; // Time step
    casadi::DM init_u;
    casadi::DM init_p;
    casadi::DM init_v;
    casadi::DM init_q;

    casadi::DM init_omega;
    casadi::Slice all;
    double m;
    double t2w;
    casadi::DM wrench2thrust;
    casadi::DM I;    // inertia
    double g = 9.81; // gravity
};
#endif
