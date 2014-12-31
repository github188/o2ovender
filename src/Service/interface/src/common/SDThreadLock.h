#ifndef SD_PTHREAD_LOCK_H
#define SD_PTHREAD_LOCK_H

#include <pthread.h>

class SDLock
{
    public:
        SDLock()
        {
            pthread_mutex_init(&m_lock, NULL);
        }
        ~SDLock()
        {
            pthread_mutex_destroy(&m_lock);
        }
        int lock()
        {
            return pthread_mutex_lock(&m_lock);
        }
        int try_lock()
        {
            return pthread_mutex_trylock(&m_lock);
        }
        int unlock()
        {
            return pthread_mutex_unlock(&m_lock);
        }

    private:
        pthread_mutex_t m_lock;

        friend class SDAutoLock;
};

class SDAutoLock
{
    public: 
        SDAutoLock(pthread_mutex_t* lock) : m_lock(lock)
    {
        pthread_mutex_lock(m_lock);
    }
        SDAutoLock(SDLock& lock) : m_lock(&(lock.m_lock))
    {
        pthread_mutex_lock(m_lock);
    }
        ~SDAutoLock()
        {
            pthread_mutex_unlock(m_lock);
        }

    private:
        pthread_mutex_t* m_lock;
};

class SDRWLock
{
    public:
        SDRWLock()
        {
            pthread_rwlock_init(&m_rwlock, NULL);
        }
        ~SDRWLock()
        {
            pthread_rwlock_destroy(&m_rwlock);
        }
        int rdlock()
        {
            return pthread_rwlock_rdlock(&m_rwlock);
        }
        int wrlock()
        {
            return pthread_rwlock_wrlock(&m_rwlock);
        }
        int try_rdlock()
        {
            return pthread_rwlock_tryrdlock(&m_rwlock);
        }
        int try_wrlock()
        {
            return pthread_rwlock_trywrlock(&m_rwlock);
        }
        int unlock()
        {
            return pthread_rwlock_unlock(&m_rwlock);
        }

    private:
        pthread_rwlock_t m_rwlock;

        friend class SDAutoRdLock;
        friend class SDAutoWrLock;
};

class SDAutoRdLock
{
    public: 
        SDAutoRdLock(pthread_rwlock_t* lock) : m_rwlock(lock)
    {
        pthread_rwlock_rdlock(m_rwlock);
    }
        SDAutoRdLock(SDRWLock& rwlock) : m_rwlock(&(rwlock.m_rwlock))
    {
        pthread_rwlock_rdlock(m_rwlock);
    }
        ~SDAutoRdLock()
        {
            pthread_rwlock_unlock(m_rwlock);
        }

    private:
        pthread_rwlock_t* m_rwlock;
};

class SDAutoWrLock
{
    public: 
        SDAutoWrLock(pthread_rwlock_t* rwlock) : m_rwlock(rwlock)
    {
        pthread_rwlock_wrlock(m_rwlock);
    }
        SDAutoWrLock(SDRWLock& rwlock) : m_rwlock(&(rwlock.m_rwlock))
    {
        pthread_rwlock_wrlock(m_rwlock);
    }
        ~SDAutoWrLock()
        {
            pthread_rwlock_unlock(m_rwlock);
        }

    private:
        pthread_rwlock_t* m_rwlock;
};

#endif /* end SD_PTHREAD_LOCK_H */


