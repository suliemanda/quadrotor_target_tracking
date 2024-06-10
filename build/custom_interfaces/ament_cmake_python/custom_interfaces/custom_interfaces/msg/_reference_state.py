# generated from rosidl_generator_py/resource/_idl.py.em
# with input from custom_interfaces:msg/ReferenceState.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import math  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_ReferenceState(type):
    """Metaclass of message 'ReferenceState'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('custom_interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'custom_interfaces.msg.ReferenceState')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__reference_state
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__reference_state
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__reference_state
            cls._TYPE_SUPPORT = module.type_support_msg__msg__reference_state
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__reference_state

            from geometry_msgs.msg import Pose
            if Pose.__class__._TYPE_SUPPORT is None:
                Pose.__class__.__import_type_support__()

            from geometry_msgs.msg import Twist
            if Twist.__class__._TYPE_SUPPORT is None:
                Twist.__class__.__import_type_support__()

            from std_msgs.msg import Header
            if Header.__class__._TYPE_SUPPORT is None:
                Header.__class__.__import_type_support__()

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class ReferenceState(metaclass=Metaclass_ReferenceState):
    """Message class 'ReferenceState'."""

    __slots__ = [
        '_header',
        '_current_pose',
        '_current_twist',
        '_n',
        '_dt',
        '_future_pose',
        '_future_twist',
    ]

    _fields_and_field_types = {
        'header': 'std_msgs/Header',
        'current_pose': 'geometry_msgs/Pose',
        'current_twist': 'geometry_msgs/Twist',
        'n': 'uint32',
        'dt': 'float',
        'future_pose': 'sequence<geometry_msgs/Pose>',
        'future_twist': 'sequence<geometry_msgs/Twist>',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.NamespacedType(['std_msgs', 'msg'], 'Header'),  # noqa: E501
        rosidl_parser.definition.NamespacedType(['geometry_msgs', 'msg'], 'Pose'),  # noqa: E501
        rosidl_parser.definition.NamespacedType(['geometry_msgs', 'msg'], 'Twist'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint32'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.NamespacedType(['geometry_msgs', 'msg'], 'Pose')),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.NamespacedType(['geometry_msgs', 'msg'], 'Twist')),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        from std_msgs.msg import Header
        self.header = kwargs.get('header', Header())
        from geometry_msgs.msg import Pose
        self.current_pose = kwargs.get('current_pose', Pose())
        from geometry_msgs.msg import Twist
        self.current_twist = kwargs.get('current_twist', Twist())
        self.n = kwargs.get('n', int())
        self.dt = kwargs.get('dt', float())
        self.future_pose = kwargs.get('future_pose', [])
        self.future_twist = kwargs.get('future_twist', [])

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.header != other.header:
            return False
        if self.current_pose != other.current_pose:
            return False
        if self.current_twist != other.current_twist:
            return False
        if self.n != other.n:
            return False
        if self.dt != other.dt:
            return False
        if self.future_pose != other.future_pose:
            return False
        if self.future_twist != other.future_twist:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def header(self):
        """Message field 'header'."""
        return self._header

    @header.setter
    def header(self, value):
        if __debug__:
            from std_msgs.msg import Header
            assert \
                isinstance(value, Header), \
                "The 'header' field must be a sub message of type 'Header'"
        self._header = value

    @builtins.property
    def current_pose(self):
        """Message field 'current_pose'."""
        return self._current_pose

    @current_pose.setter
    def current_pose(self, value):
        if __debug__:
            from geometry_msgs.msg import Pose
            assert \
                isinstance(value, Pose), \
                "The 'current_pose' field must be a sub message of type 'Pose'"
        self._current_pose = value

    @builtins.property
    def current_twist(self):
        """Message field 'current_twist'."""
        return self._current_twist

    @current_twist.setter
    def current_twist(self, value):
        if __debug__:
            from geometry_msgs.msg import Twist
            assert \
                isinstance(value, Twist), \
                "The 'current_twist' field must be a sub message of type 'Twist'"
        self._current_twist = value

    @builtins.property
    def n(self):
        """Message field 'n'."""
        return self._n

    @n.setter
    def n(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'n' field must be of type 'int'"
            assert value >= 0 and value < 4294967296, \
                "The 'n' field must be an unsigned integer in [0, 4294967295]"
        self._n = value

    @builtins.property
    def dt(self):
        """Message field 'dt'."""
        return self._dt

    @dt.setter
    def dt(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'dt' field must be of type 'float'"
            assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                "The 'dt' field must be a float in [-3.402823466e+38, 3.402823466e+38]"
        self._dt = value

    @builtins.property
    def future_pose(self):
        """Message field 'future_pose'."""
        return self._future_pose

    @future_pose.setter
    def future_pose(self, value):
        if __debug__:
            from geometry_msgs.msg import Pose
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 all(isinstance(v, Pose) for v in value) and
                 True), \
                "The 'future_pose' field must be a set or sequence and each value of type 'Pose'"
        self._future_pose = value

    @builtins.property
    def future_twist(self):
        """Message field 'future_twist'."""
        return self._future_twist

    @future_twist.setter
    def future_twist(self, value):
        if __debug__:
            from geometry_msgs.msg import Twist
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 all(isinstance(v, Twist) for v in value) and
                 True), \
                "The 'future_twist' field must be a set or sequence and each value of type 'Twist'"
        self._future_twist = value
