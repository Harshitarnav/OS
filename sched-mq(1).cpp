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
    /*To disable the interrupts when manipulating the run queue*/
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
		assert((entity.priority() == SchedulingEntityPriority::REALTIME) || (entity.priority() == SchedulingEntityPriority::INTERACTIVE) || (entity.priority() == SchedulingEntityPriority::DAEMON) || (entity.priority() ==SchedulingEntityPriority::NORMAL));
	}

    }

    /**
     * Called when a scheduling entity is no longer eligible for running.
     * @param entity
     */
    void remove_from_runqueue(SchedulingEntity& entity) override
    {
        /*To disable the interrupts when manipulating the run queue*/
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
	    /*To disable the interrupts when manipulating the run queue*/
	    UniqueIRQLock l;
        
      	if (!realtime.empty()) {
		    return round_robin(realtime);
	  	}
        
      	else if (!interactive.empty()) {
        	return round_robin(interactive);
	  	}
        
      	else if (!normal.empty()) {
        	return round_robin(normal);
	  	}
        
      	else if (!daemon.empty()) {
        	return round_robin(daemon);
	  	}
        
        /*For idle process to run*/
      	else {
		    return NULL;
	  	}
    }

    /**
     * Implements the Round Robin algorithm.
     * @param prio_queue The queue used for round robin algorithm.
     * @return returns the next entity in the queue.
     */
    SchedulingEntity *round_robin(List<SchedulingEntity *> &prio_queue) 
    {
        /*Stores the task to be returned*/
        SchedulingEntity *priority_entity = prio_queue.pop();
		prio_queue.enqueue(priority_entity);
        return priority_entity;
    }

private:
  List<SchedulingEntity *> realtime;
  List<SchedulingEntity *> interactive;
  List<SchedulingEntity *> normal;
  List<SchedulingEntity *> daemon;
};

/* --- DO NOT CHANGE ANYTHING BELOW THIS LINE --- */

RegisterScheduler(MultipleQueuePriorityScheduler);
