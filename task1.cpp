/*
 * The Priority Task Scheduler
 * SKELETON IMPLEMENTATION TO BE FILLED IN FOR TASK 1
 */

#include <infos/kernel/sched.h>
#include <infos/kernel/thread.h>
#include <infos/kernel/log.h>
#include <infos/util/list.h>
#include <infos/util/lock.h>

using namespace infos::kernel;
using namespace infos::util;

/**
 * A Multiple Queue priority scheduling algorithm
 */
class MultipleQueuePriorityScheduler : public SchedulingAlgorithm
{
public:
    /**
     * Returns the friendly name of the algorithm, for debugging and selection purposes.
     */
    const char* name() const override { return "mq"; }

    /**
     * Called during scheduler initialisation.
     */
    void init()
    {
        // TODO: Implement me!
    }

    /**
     * Called when a scheduling entity becomes eligible for running.
     * @param entity
     */
    void add_to_runqueue(SchedulingEntity& entity) override
    {
      UniqueIRQLock l;
      realtime.enqueue(&entity.priority());
      interactive.enqueue(&entity.priority());
      normal.enqueue(&entity.priority());
      daemon.enqueue(&entity.priority());
        // TODO: Implement me!
    }

    /**
     * Called when a scheduling entity is no longer eligible for running.
     * @param entity
     */
    void remove_from_runqueue(SchedulingEntity& entity) override
    {
      UniqueIRQLock l;
      if (!realtime.empty()) {
	realtime.remove(&entity)
	  }

      if (!interactive.empty()) {
	interactive.remove(&entity)
	  }

      if (!normal.empty()) {
	interactive.remove(&entity)
	  }

      if (!daemon.empty()) {
	daemon.remove(&entity)
	  }
        // TODO: Implement me!
    }

    /**
     * Called every time a scheduling event occurs, to cause the next eligible entity
     * to be chosen.  The next eligible entity might actually be the same entity, if
     * e.g. its timeslice has not expired.
     */
    SchedulingEntity *pick_next_entity() override
    {
      if (!realtime.empty()) {
	return realtime.first()
	  }

      if (!interactive.empty()) {
	return interactive.first()
	  }

      if (!normal.empty()) {
	return normal.first()
	  }

      if (!daemon.empty()) {
	return daemon.first()
	  }

      else {
	return NULL;
      }
        // TODO: Implement me!
    }
private:
  List<SchedulingEntity *> realtime;
  List<SchedulingEntity *> interactive;
  List<SchedulingEntity *> normal;
  List<SchedulingEntity *> daemon;
};

/* --- DO NOT CHANGE ANYTHING BELOW THIS LINE --- */

RegisterScheduler(MultipleQueuePriorityScheduler);



File Edit Options Buffers Tools C++ Help                                        
      else if (!daemon.empty()) {
        return daemon.pop()
          }

      else {
        return NULL
          }
        // TODO: Implement me!                                                  
    }

private:
  List<SchedulingEntity *> realtime;
  List<SchedulingEntity *> interactive;
  List<SchedulingEntity *> normal;
  List<SchedulingEntity *> daemon;
};

/* --- DO NOT CHANGE ANYTHING BELOW THIS LINE --- */

RegisterScheduler(MultipleQueuePriorityScheduler);

-UU-:**--F1  sched-mq.cpp   Bot L116  Git-main  (C++//l Abbrev) ----------------
Auto-saving...done










File Edit Options Buffers Tools Debugger Help                                   
/*                                                                              
 * The Priority Task Scheduler                                                  
 * SKELETON IMPLEMENTATION TO BE FILLED IN FOR TASK 1                           
 */




void add_to_runqueue(SchedulingEntity& entity) override
    {
      UniqueIRQLock l;

      if (&entity.priority() == REALTIME) {
        realtime.enqueue(&entity)
          }
      else if (&entity.priority() == INTERACTIVE) {
        interactive.enqueue(&entity)
          }
      else if (&entity.priority() == DAEMON) {
        daemon.enqueue(&entity)
          }
      else {
        normal.enqueue(&entity)
          }
        // TODO: Implement me!                                                                                                      
    }


void remove_from_runqueue(SchedulingEntity& entity) override
    {
      UniqueIRQLock l;

      if (!realtime.empty()) {
        realtime.remove(&entity)
          }

      if (!interactive.empty()) {
        interactive.remove(&entity)
          }

      if (!normal.empty()) {
        interactive.remove(&entity)
          }

      if (!daemon.empty()) {
        daemon.remove(&entity)
          }
        // TODO: Implement me!                                                                                                      
    }


SchedulingEntity *pick_next_entity() override
    {
      else if (!realtime.empty()) {
        return realtime.pop()
          }

      else if (!interactive.empty()) {
        return interactive.pop()
          }

      else if (!normal.empty()) {
        return normal.pop()
          }

      else if (!daemon.empty()) {
        return daemon.pop()
          }

      else {
        return NULL
          }
        // TODO: Implement me!                                                                                                      
    }

private:
  List<SchedulingEntity *> realtime;
  List<SchedulingEntity *> interactive;
  List<SchedulingEntity *> normal;
  List<SchedulingEntity *> daemon;







/*OA
 * The Priority Task Scheduler
 * SKELETON IMPLEMENTATION TO BE FILLED IN FOR TASK 1
 */

#include <infos/kernel/sched.h>
#include <infos/kernel/thread.h>
#include <infos/kernel/log.h>
#include <infos/util/list.h>
#include <infos/util/lock.h>

using namespace infos::kernel;
using namespace infos::util;

/**
 * A Multiple Queue priority scheduling algorithm
 */
class MultipleQueuePriorityScheduler : public SchedulingAlgorithm
{
public:
    /**
     * Returns the friendly name of the algorithm, for debugging and selection purposes.
     */
    const char* name() const override { return "mq"; }

    /**
     * Called during scheduler initialisation.
     */
    void init()
    {
        // TODO: Implement me!
    }

    /**
     * Called when a scheduling entity becomes eligible for running.
     * @param entity
     */
    void add_to_runqueue(SchedulingEntity& entity) override
    {
      UniqueIRQLock l;
      
      if (entity.priority() == SchedulingEntityPriority::REALTIME) {
	realtime.enqueue(&entity)
	  }
      else if (entity.priority() == SchedulingEntityPriority::INTERACTIVE) {
	interactive.enqueue(&entity)
	  }
      else if (entity.priority() == SchedulingEntityPriority::DAEMON) {
	daemon.enqueue(&entity)
	  }
      else {
	normal.enqueue(&entity)
	  }
	syslog.messagef(LogLevel::DEBUG, "AAAAAAA");
        // TODO: Implement me!
    }

    /**
     * Called when a scheduling entity is no longer eligible for running.
     * @param entity
     */
    void remove_from_runqueue(SchedulingEntity& entity) override
    {
      UniqueIRQLock l;

      if (entity.priority() == SchedulingEntityPriority::REALTIME) {
	realtime.remove(&entity)
	  }

      if (entity.priority() == SchedulingEntityPriority::INTERACTIVE) {
	interactive.remove(&entity)
	  }

      if (entity.priority() == SchedulingEntityPriority::NORMAL) {
	normal.remove(&entity)
	  }

      if (entity.priority() == SchedulingEntityPriority::DAEMON) {
	daemon.remove(&entity)
	  }
	syslog.messagef(LogLevel::DEBUG, "BBBBBBBBB");
        // TODO: Implement me!
    }

    /**
     * Called every time a scheduling event occurs, to cause the next eligible entity
     * to be chosen.  The next eligible entity might actually be the same entity, if
     * e.g. its timeslice has not expired.
     */
    SchedulingEntity *pick_next_entity() override
    {
      if (!realtime.empty()) {
        realtime.enqueue(realtime.first())
	return realtime.pop()
    
	  }

      else if (!interactive.empty()) {
        interactive.enqueue(interactive.first())
	return interactive.pop()
	  }

      else if (!normal.empty()) {
        normal.enqueue(normal.first())
	return normal.pop()
	  }

      else if (!daemon.empty()) {
        daemon.enqueue(daemon.first())
	return daemon.pop()
	  }

      else {
	return NULL
	  }
	syslog.messagef(LogLevel::DEBUG, "CCCCCCCCC");
        // TODO: Implement me!
    }

private:
  List<SchedulingEntity *> realtime;
  List<SchedulingEntity *> interactive;
  List<SchedulingEntity *> normal;
  List<SchedulingEntity *> daemon;
};

/* --- DO NOT CHANGE ANYTHING BELOW THIS LINE --- */

RegisterScheduler(MultipleQueuePriorityScheduler);