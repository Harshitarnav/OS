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
        //not used
    }

     /**
     * Called when a scheduling entity becomes eligible for running.
     * @param entity task to be added to a run queue
     */
    void add_to_runqueue(SchedulingEntity& entity) override
    {
		/*Error handling entity to be added to queue should not be null*/
		if(&entity==NULL) {
            syslog.messagef(LogLevel::ERROR, "Trying to add NULL entity to queue");
            return;
        }
        /*To make sure that interrupts are disabled when manipulating the queues*/
		UniqueIRQLock l;
		
		/*Enqueuing different runqueues depending on priority of the tasks, 
		Tasks are added to the normal queue by default if priority is not explicitly mentioned*/

		if( entity.priority() == SchedulingEntityPriority::REALTIME)
		  realtimequeue.enqueue(&entity);
	    else if( entity.priority() == SchedulingEntityPriority::INTERACTIVE)
		  interactivequeue.enqueue(&entity);
	  	else if( entity.priority() == SchedulingEntityPriority::DAEMON)
	      daemonqueue.enqueue(&entity);
	    else
	      normalqueue.enqueue(&entity);
    }

    /**
     * Called when a scheduling entity is no longer eligible for running.
     * @param entity task to be removed from a run queue
     */
    void remove_from_runqueue(SchedulingEntity& entity) override
    {
		/*Error handling entity to be removed from queue cannot be null*/
		if(&entity==NULL)
			syslog.messagef(LogLevel::ERROR, "Trying to remove NULL entity from queue");
		/*To make sure that interrupts are disabled when manipulating the queues*/
		UniqueIRQLock l;
		
		/*Removing from different runqueues depending on priority of the tasks, 
		Tasks without an explicit priority by default lie in the normal queue hence removed from there*/
		
		if( entity.priority() == SchedulingEntityPriority::REALTIME)
		  realtimequeue.remove(&entity);
	    else if( entity.priority() == SchedulingEntityPriority::INTERACTIVE)
		  interactivequeue.remove(&entity);
	  	else if( entity.priority() == SchedulingEntityPriority::DAEMON)
	      daemonqueue.remove(&entity);
	    else
	      normalqueue.remove(&entity);
    }

    /**
     * Called every time a scheduling event occurs, to cause the next eligible entity
     * to be chosen.  The next eligible entity might actually be the same entity, if
     * e.g. its timeslice has not expired.
	 * @return the next eligible entity for scheduling according to scheduling algorithm
     */
    SchedulingEntity *pick_next_entity() override
    {
		//Return null if all queues are empty (make the idle process to run)
		if (realtimequeue.count()==0 && interactivequeue.count()==0 && normalqueue.count()==0 && daemonqueue.count() == 0) return NULL;
		
		//Error handling none of the queues can have a negative size 
		assert((realtimequeue.count()>=0) &&(interactivequeue.count()>=0) && (normalqueue.count()>=0) && (daemonqueue.count()>=0));
		

		/*To make sure that interrupts are disabled when manipulating the queues*/
		UniqueIRQLock l;
		/** Choosing next entity from different queues based on priority*/
		/** Realtime priority*/
	    if(realtimequeue.count()>0)
			return pop_and_enqueue(realtimequeue);
		/** Interactive priority*/
		else if(interactivequeue.count()>0)
			return pop_and_enqueue(interactivequeue);
		/** Normal priority*/
		else if(normalqueue.count()>0)
            return pop_and_enqueue(normalqueue);
		/** Daemon priority*/
		else if(daemonqueue.count()>0)
            return pop_and_enqueue(daemonqueue);
		
		//Error handling : Return null which will cause the idle process to run 
		return NULL;
    }
	/**
	* Function to return scheduling entity after popping and re-enquing in queue(round-robin)
	*@param queue the queue to be popped from and re-enqueued
	*@return the next eligible entity for running in queue
	*/
	SchedulingEntity *pop_and_enqueue(List<SchedulingEntity *> &queue){
		// To store the number of tasks, used for error handling
		unsigned int count=queue.count();
		// To store the task to be returned
		SchedulingEntity *priority_entity =queue.pop();

		queue.enqueue(priority_entity);
		//Error handling to check if task was lost during poping and re-enquing
		if(queue.count()!=count)
			syslog.messagef(LogLevel::ERROR, "Task has been lost in queue");
		return priority_entity;
	}
	private:
	/**Queue declarations based on different priorities*/
	List<SchedulingEntity *> realtimequeue;
	List<SchedulingEntity *> interactivequeue;
	List<SchedulingEntity *> normalqueue;
	List<SchedulingEntity *> daemonqueue;
};

/* --- DO NOT CHANGE ANYTHING BELOW THIS LINE --- */

RegisterScheduler(MultipleQueuePriorityScheduler);





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
      else {
	normal.enqueue(&entity);
	  }
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
        // TODO: Implement me!
    }

    /**
     * Called every time a scheduling event occurs, to cause the next eligible entity
     * to be chosen.  The next eligible entity might actually be the same entity, if
     * e.g. its timeslice has not expired.
     */
    SchedulingEntity *pick_next_entity() override
    {
      UniqueIRQLock l;
    
      SchedulingEntity *a;
     if (realtime.count()>0) {
      	
      	// realtime.enqueue(realtime.first());
	// return realtime.pop();
      	a = realtime.pop();
        realtime.enqueue(a);
	return a;
	  }

      else if (interactive.count()>0) {
        a = interactive.pop();
        interactive.enqueue(a);
	return a;
	  }

      else if (normal.count()>0) {
        a = normal.pop();
        normal.enqueue(a);
	return a;
	  }

      else if (daemon.count()>0) {
        a = daemon.pop();
        daemon.enqueue(a);
	return a;
	  }

      else {
	return NULL;
	  } 
    }
};

/* --- DO NOT CHANGE ANYTHING BELOW THIS LINE --- */

RegisterScheduler(MultipleQueuePriorityScheduler);