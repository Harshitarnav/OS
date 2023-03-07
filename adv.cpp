// /*
//  * The Advanced Task Scheduler
//  * SKELETON IMPLEMENTATION TO BE FILLED IN FOR TASK 1
//  */
//
// #include <infos/kernel/sched.h>
// #include <infos/kernel/thread.h>
// #include <infos/kernel/log.h>
// #include <infos/util/list.h>
// #include <infos/util/lock.h>
//
// using namespace infos::kernel;
// using namespace infos::util;
//
// /**
//  * A Multiple Queue priority scheduling with modifications algorithm
//  */
// class AdvancedScheduler : public SchedulingAlgorithm
// {
// public:
//     /**
//      * Returns the friendly name of the algorithm, for debugging and selection purposes.
//      */
//     const char* name() const override { return "adv"; }
//
//     /**
//      * Called during scheduler initialisation.
//      */
//     void init()
//     {
//         time=0;
//     }
//
//     /**
//      * Called when a scheduling entity becomes eligible for running.
//      * @param entity task to be added to a run queue
//      */
//     void add_to_runqueue(SchedulingEntity& entity) override
//     {
// 		/*Error handling entity to be added to queue should not be null*/
// 		if(&entity==NULL) {
//             syslog.messagef(LogLevel::ERROR, "Trying to add NULL entity to queue");
//             return;
//         }
//         /*To make sure that interrupts are disabled when manipulating the queues*/
// 		UniqueIRQLock l;
//
// 		/*Enqueuing different runqueues depending on priority of the tasks,
// 		Tasks are added to the normal queue by default if priority is not explicitly mentioned*/
//
// 		if( entity.priority() == SchedulingEntityPriority::REALTIME)
// 		  realtimequeue.enqueue(&entity);
// 	    else if( entity.priority() == SchedulingEntityPriority::INTERACTIVE)
// 		  interactivequeue.enqueue(&entity);
// 	  	else if( entity.priority() == SchedulingEntityPriority::DAEMON)
// 	      daemonqueue.enqueue(&entity);
// 	    else
// 	      normalqueue.enqueue(&entity);
//
//     }
//
//     /**
//      * Called when a scheduling entity is no longer eligible for running.
//      * @param entity task to be removed from a run queue
//      */
//     void remove_from_runqueue(SchedulingEntity& entity) override
//     {
// 		/*Error handling entity to be removed from queue cannot be null*/
// 		if(&entity==NULL)
// 			syslog.messagef(LogLevel::ERROR, "Trying to remove NULL entity from queue");
//
// 		/*To make sure that interrupts are disabled when manipulating the queues*/
// 		UniqueIRQLock l;
//
// 		/*Removing from different runqueues depending on priority of the tasks,
// 		Tasks without an explicit priority by default lie in the normal queue hence removed from there*/
//
// 		if( entity.priority() == SchedulingEntityPriority::REALTIME)
// 		  realtimequeue.remove(&entity);
// 	    else if( entity.priority() == SchedulingEntityPriority::INTERACTIVE)
// 		  interactivequeue.remove(&entity);
// 	  	else if( entity.priority() == SchedulingEntityPriority::DAEMON)
// 	      daemonqueue.remove(&entity);
// 	    else
// 	      normalqueue.remove(&entity);
//     }
//
//     /**
//      * Called every time a scheduling event occurs, to cause the next eligible entity
//      * to be chosen.  The next eligible entity might actually be the same entity, if
//      * e.g. its timeslice has not expired.
// 	 * @return the next eligible entity for scheduling according to scheduling algorithm
//      */
//     SchedulingEntity *pick_next_entity() override
//     {
// 		//Return null if all queues are empty (make the idle process to run)
// 		if (realtimequeue.count()==0 && interactivequeue.count()==0 && normalqueue.count()==0 && daemonqueue.count() == 0) return NULL;
// 		//Error handling none of the queues can have a negative size
// 		assert((realtimequeue.count()>=0) &&(interactivequeue.count()>=0) && (normalqueue.count()>=0) && (daemonqueue.count()>=0));
//
// 		/* Timer used for dividing time slice for queues*/
// 		time= time %101;
// 		time++;
// 		/*To make sure that interrupts are disabled when manipulating the queues*/
// 		UniqueIRQLock l;
// 		/* Pick next task based on timing*/
// 		SchedulingEntity *priority_entity = entity_picker(false);
// 		/*If no task in queue based on timing, choose based on priority*/
// 		if( priority_entity ==NULL){
// 			priority_entity= entity_picker(true);
// 		}
// 		return priority_entity;
//
//     }
// 	/**
// 	* Called to choose next entity from different queues based on priority
// 	* @param use_time Indicates whether to use timing information
// 	* for selection of queue,(false to use timing information)
// 	* (true to not use timing information)
// 	* @return choose next entity from different queues based on priority and/or timing information
// 	*/
// 	SchedulingEntity *entity_picker(bool use_time){
//
// 		/**Choosing next entity from different queues based on priority and/or timing information*/
// 		/** Realtime priority- Round Robin*/
// 		if(realtimequeue.count()>0 && (time<=50 || use_time) ) {
//           syslog.messagef(LogLevel::DEBUG, "1");
//            return pop_and_enqueue(realtimequeue);
//          }
// 		/** Interactive priority- Round Robin*/
// 		else if(interactivequeue.count()>0 && ((time>50 && time<=80) || use_time)) {
//       syslog.messagef(LogLevel::DEBUG, "2");
//            return pop_and_enqueue(interactivequeue);
//          }
// 		/** Normal priority- Completely fair scheduler (basic)*/
// 		else if(normalqueue.count()>0 && ((time >80 &&time<=95) || use_time)) {
//       syslog.messagef(LogLevel::DEBUG, "3");
// 			return cfs_normal_queue(normalqueue);
//     }
// 		/** Daemon priority- First come first serve*/
// 		else if(daemonqueue.count()>0 && ((time>95 && time<=100) || use_time)) {
//       syslog.messagef(LogLevel::DEBUG, "4");
// 			return daemonqueue.first();
//     }
//
// 		//Error handling : Return null which will cause the idle process to run
// 		return NULL;
// 	}
// 	/**
// 	 * Implementing a poor version of the completely fair scheduler for the normal queue
// 	 * @param queue The queue to be used for the cfs algorithm
// 	 * @return chooses next task to run in normal queue based on the task which has got the least runtime
// 	 */
// 	SchedulingEntity *cfs_normal_queue(List<SchedulingEntity *> &queue){
//         /* if only one task in queue return the task*/
// 		if (queue.count() == 1) return queue.first();
//
// 		SchedulingEntity::EntityRuntime min_runtime = 0;
// 		/* To store entity to be returned, Initialized with NULL for *ERROR HANDLING*/
// 		SchedulingEntity *min_runtime_entity = NULL;
// 		/* Choosing the task which has got the least CPU runtime*/
// 		for (const auto& entity : queue) {
// 			if (min_runtime_entity == NULL || entity->cpu_runtime() < min_runtime) {
// 				min_runtime_entity = entity;
// 				min_runtime = entity->cpu_runtime();
// 			}
// 		}
// 		return min_runtime_entity;
// 	}
// 	/**
// 	* Function to return scheduling entity after popping and re-enquing in queue(round-robin)
// 	*@param queue  the queue to be popped from and re-enqueued
// 	*@return the next eligible entity for running in queue
// 	*/
// 	SchedulingEntity *pop_and_enqueue(List<SchedulingEntity *> &queue){
// 		// To store the number of tasks used for error handling
// 		unsigned int count=queue.count();
//
// 		// To store the task to be returned
// 		SchedulingEntity *priority_entity =queue.pop();
// 		queue.enqueue(priority_entity);
// 		//Error handling to check if task was lost during poping and re-enquing
// 		if(queue.count()!=count)
// 			syslog.messagef(LogLevel::ERROR, "Task has been lost in queue");
// 		return priority_entity;
// 	}
//
// 	private:
// 	/**Queue declarations based on different priorities*/
// 	List<SchedulingEntity *> realtimequeue;
// 	List<SchedulingEntity *> interactivequeue;
// 	List<SchedulingEntity *> normalqueue;
// 	List<SchedulingEntity *> daemonqueue;
// 	/*Store timing information important for time slice for each queue*/
// 	int time;
// };
//
// /* --- DO NOT CHANGE ANYTHING BELOW THIS LINE --- */
//
// RegisterScheduler(AdvancedScheduler);


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

          /*Round Robin for Realtime priority queue and time slice of 0-40*/
          if (!realtime.empty()) {
            return round_robin(realtime);
          }

          /*Round Robin for Interactive priority queue and time slice of 40-70*/
          else if (!interactive.empty()) {
            return round_robin(interactive);
          }

          /*Shortest Job First for Normal priority queue and time slice of 70-90*/
          else if (!normal.empty()) {
            return shortest_job_first(normal);
          }

          /*Shortest Job First for Daemon priority queue and time slice of 90-100*/
          else if (!daemon.empty()) {
            return shortest_job_first(daemon);
          }
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
        if (prio_queue.count()==1) {
          return prio_queue.first();
        }

        /*Initialising a variable min_time for getting the entity which takes the minimum cpu time*/
        SchedulingEntity::EntityRuntime min_time = 0;

        /*Stores the entity that takes the shortest duration to complete*/
        SchedulingEntity *min_entity = NULL;

        for (const auto& entity : prio_queue) {
            if (min_entity==NULL || entity->cpu_runtime()<min_time) {
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
