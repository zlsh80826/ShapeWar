cdef class MovableObject:

    cdef public double friction
    cdef public int x_bound
    cdef public int y_bound
    cdef public complex velocity
    cdef public complex pos
    cdef public double radius

    cpdef void spawn(self)

    cpdef void apply_friction(self)

    cpdef void accelerate(self, acc)

    cpdef void limit_pos(self)
