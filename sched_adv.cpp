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
class AdvanceScheduler : public SchedulingAlgorithm
{
public:
    /**
     * Returns the friendly name of the algorithm, for debugging and selection purposes.
     */
    const char* name() const override { return "adv"; }

    /**
     * Called during scheduler initialisation.
     */
    void init()
    {
        time=0;
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
        /*Timer for providing time slices for priority queues*/
        if (time>100) {
            time = 0;
        }
        time++;

        /*To disable the interrupts when manipulating the run queue*/
	    UniqueIRQLock l;
        
        /*Round Robin for Realtime priority queue and time slice of 0-40*/
      	if (!realtime.empty() && (time<=40)) {
		    return round_robin(realtime);
	  	}
        
        /*Round Robin for Interactive priority queue and time slice of 40-70*/
      	else if (!interactive.empty() && (time>40 && time<=70)) {
        	return round_robin(interactive);
	  	}
        
        /*Shortest Job First for Normal priority queue and time slice of 70-90*/
      	else if (!normal.empty() && (time>70 && time<=90)) {
        	return shortest_job_first(normal);
	  	}
        
        /*Shortest Job First for Daemon priority queue and time slice of 90-100*/
      	else if (!daemon.empty() && (time>90 && time<=100)) {
        	return shortest_job_first(daemon);
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

    /**
     * Implements the Shortest Job First algorithm.
     * @param prio_queue The queue used for sjf algorithm.
     * @return returns the entity that takes the shortest cpu time.
     */
    SchedulingEntity *shortest_job_first(List<SchedulingEntity *> &prio_queue) 
    {
        /*Initialising a variable min_time for getting the entity which takes the minimum cpu time*/
        SchedulingEntity::EntityRuntime min_time = prio_queue.first()->cpu_runtime();

        /*Stores the entity that takes the shortest duration to complete*/
        SchedulingEntity *min_entity = NULL;

        for (const auto& entity : prio_queue) {
            if (entity->cpu_runtime()<min_time) {
                min_entity = entity;
                min_time = entity->cpu_runtime();
            }
        }
        return min_entity;
    }

private:
  /*All priority queues*/
  List<SchedulingEntity *> realtime;
  List<SchedulingEntity *> interactive;
  List<SchedulingEntity *> normal;
  List<SchedulingEntity *> daemon;
  
  /*For providing different time slices to each queue*/
  int time;
};

/* --- DO NOT CHANGE ANYTHING BELOW THIS LINE --- */

RegisterScheduler(AdvanceScheduler);
