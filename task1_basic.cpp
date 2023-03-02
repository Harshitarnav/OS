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
      
	if (entity.priority() == SchedulingEntityPriority::REALTIME) {
		realtime.enqueue(&entity);
	}
	else if (entity.priority() == SchedulingEntityPriority::INTERACTIVE) {
		interactive.enqueue(&entity);
	}
    	else if (entity.priority() == SchedulingEntityPriority::DAEMON) {
		daemon.enqueue(&entity);
	}
    	else if (entity.priority() == SchedulingEntityPriority::NORMAL) {
		normal.enqueue(&entity);
	}
	else {
		assert((entity.priority() == SchedulingEntityPriority::REALTIME) || 			(entity.priority() == SchedulingEntityPriority::INTERACTIVE) || (entity.priority() 			== SchedulingEntityPriority::DAEMON) || (entity.priority() 				==SchedulingEntityPriority::NORMAL));
	}

    }

    /**
     * Called when a scheduling entity is no longer eligible for running.
     * @param entity
     */
    void remove_from_runqueue(SchedulingEntity& entity) override
    {
      UniqueIRQLock l;

      if (entity.priority() == SchedulingEntityPriority::REALTIME) {
	realtime.remove(&entity);
	  }

      if (entity.priority() == SchedulingEntityPriority::INTERACTIVE) {
	interactive.remove(&entity);
	  }

      if (entity.priority() == SchedulingEntityPriority::NORMAL) {
	normal.remove(&entity);
	  }

      if (entity.priority() == SchedulingEntityPriority::DAEMON) {
	daemon.remove(&entity);
	  }
    }

    /**
     * Called every time a scheduling event occurs, to cause the next eligible entity
     * to be chosen.  The next eligible entity might actually be the same entity, if
     * e.g. its timeslice has not expired.
     */
    SchedulingEntity *pick_next_entity() override
    {
	UniqueIRQLock l;

      	if (!realtime.empty()) {
        	SchedulingEntity *priority_entity = realtime.pop();
		realtime.enqueue(priority_entity);
		return priority_entity;
	  	}

      	else if (!interactive.empty()) {
        	SchedulingEntity *priority_entity = interactive.pop();
		interactive.enqueue(priority_entity);
		return priority_entity;
	  	}

      	else if (!normal.empty()) {
        	SchedulingEntity *priority_entity = normal.pop();
		normal.enqueue(priority_entity);
		return priority_entity;
	  	}

      	else if (!daemon.empty()) {
        	SchedulingEntity *priority_entity = daemon.pop();
		daemon.enqueue(priority_entity);
		return priority_entity;
	  	}

      	else {
		return NULL;
	  	}
    }

private:
  List<SchedulingEntity *> realtime;
  List<SchedulingEntity *> interactive;
  List<SchedulingEntity *> normal;
  List<SchedulingEntity *> daemon;
};

/* --- DO NOT CHANGE ANYTHING BELOW THIS LINE --- */

RegisterScheduler(MultipleQueuePriorityScheduler);
