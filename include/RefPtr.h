#ifndef REFPTR_H
#define REFPTR_H

#include "RefCounted.h"

namespace plv {

/**
 * RefPtr template is a helper class for reference counted objects. RefPtr objects
 * should be allocated on the stack. Assigning a reference counted object to it will
 * trigger RefPtr to automatically call the inc() method on the object to increase
 * its reference count. The destructor of RefPtr automatically calls the dec()
 * method, decreasing the reference count of the reference counted object.
 */
template< class T >
class RefPtr
{

public:
    RefPtr() : m_ptr( 0 ) {}

    RefPtr( T* ptr ) : m_ptr( ptr )
    {
        if( m_ptr )
        {
            m_ptr->inc();
        }
    }

    /** copy constructor */
    RefPtr( const RefPtr<T>& rp )
    {
        m_ptr = rp.getPtr();
        if( m_ptr != 0 )
        {
            m_ptr->inc();
        }
    }

    /**
     * Destructor. Decreases the reference count of m_ptr. The reference counted
     * object will delete itself when its reference count reaches zero. The
     * destructor is automatically called when RefPtr goes out of scope, thus
     * ensuring non referenced objects are deleted.
     */
    ~RefPtr()
    {
        if( m_ptr != 0 )
        {
            m_ptr->dec();
            m_ptr = 0;
        }
    }

    /** set the pointer to the reference counted object. This replaces an old
      * pointer if set
      */
    void set( T* obj )
    {
        if( m_ptr != 0 )
        {
            m_ptr->dec();
        }
        m_ptr = obj;
    }

    /** Returns a pointer to the managed object with implicit output
      *  conversion
      */
    inline operator T*() const { return m_ptr; }

    /**
      * Returns the C-style pointer to the RefCounted object assigned to
      * this RefPtr
      */
    T* getPtr() const
    {
        return m_ptr;
    }

    RefPtr& operator = (const RefPtr& rp)
    {
        // protect against self assignment
        if( m_ptr != rp.m_ptr )
        {
            // save old pointer temporarily
            T* tmp = m_ptr;

            // assign new pointer and increase reference count
            m_ptr = rp.m_ptr;
            if( m_ptr != 0 )
            {
                m_ptr->inc();
            }

            // now decrease reference count on the old pointer
            // we do this after increasing the reference count on
            // the new pointer, to avoid situations where the new
            // ptr would get deleted, because it was a child of the
            // old ptr.
            if( tmp != 0 )
            {
                tmp->dec();
            }
        }
        return *this;
    }

    template<class Other> RefPtr& operator = (const RefPtr<Other>& rp)
    {
        // protect against self assignment
        if( m_ptr != rp.m_ptr )
        {
            // save old pointer temporarily
            T* tmp = m_ptr;

            // assign new pointer and increase reference count
            m_ptr = rp.m_ptr;
            if( m_ptr != 0 )
            {
                m_ptr->inc();
            }

            // now decrease reference count on the old pointer
            // we do this after increasing the reference count on
            // the new pointer, to avoid situations where the new
            // ptr would get deleted, because it was a child of the
            // old ptr.
            if( tmp != 0 )
            {
                tmp->dec();
            }
        }
        return *this;
    }

    /** Assignment operator. Replaces an old managed object if one is present
      * and lowers it reference count
      */
    inline RefPtr& operator = ( T* ptr )
    {
        // protect against self assignment
        if( m_ptr != ptr )
        {
            // save old pointer temporarily
            T* tmp = m_ptr;

            // assign new pointer and increase reference count
            m_ptr = ptr;
            if( m_ptr != 0 )
            {
                m_ptr->inc();
            }

            // now decrease reference count on the old pointer
            // we do this after increasing the reference count on
            // the new pointer, to avoid situations where the new
            // ptr would get deleted, because it was a child of the
            // old ptr.
            if( tmp != 0 )
            {
                tmp->dec();
            }
        }
        return *this;
    }

    /** compares pointer values of managed RefCounted objects if both pointers
      * are not equal to 0.
      */
    bool operator == ( const RefPtr<T>& rhs ) const
    {
        return m_ptr != 0 ? rhs.getObj() == m_ptr : false;
    }

    T& operator * () const { return *m_ptr; }
    T* operator ->() const { return m_ptr; }

    /** returns if the pointer to the managed object is valid (not null) */
    bool isValid() const { return m_ptr != 0; }

private:
    /**
     * Pointer to an object which inherits from the RefCounted base class
     */
    T* m_ptr;

};

} // end namespace plv

#endif // REFPTR_H
