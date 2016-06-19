cdef class MovableObject:

    cdef public double friction
    cdef public double x_min
    cdef public double x_max
    cdef public double y_min
    cdef public double y_max
    cdef public complex velocity
    cdef public complex pos
    cdef public double radius

    cpdef void spawn(self)

    cpdef void apply_friction(self)

    cpdef void bound_and_bounce(self)

    cpdef void tick_pos(self)
