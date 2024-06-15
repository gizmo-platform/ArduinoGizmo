#ifndef BasicTask_h
#define BasicTask_h

typedef int (Task)(int);
typedef void (TaskSetup)();

/* Example usage for tasks Class
  in main ino file, or your own other file, create function for Drive setup and loop
    void DriveSetup() {
      ... get ready to do cool stuff ... 
    }

    int DriveTask(int runStep) { 
      ... do cool stuff ... 
    }

  in main setup(), create your task with a name, address to a function to execute as its loop, and an optional setup function address
    void setup() {
      ... do other stuff
      Tasks.Create("DRIVE", &DriveTask, &DriveSetup);
      Tasks.Create("LEDS", &BlinkEm);
      ...
      Tasks.Create("ARM", &GoArmGo);
    }

  in main loop(), ensure Tasks class runs all of its tasks, no delay needed - tasks takes care of that as long as everything runs in a task
    void loop() {
      Tasks.Update();
    }
*/
  

// Managed subtask the TaskManager controls
class subTask {
  public:
    subTask() {_runStep = _nextTime =0; _loopFnc = NULL; _Next = NULL;}; 
    char* _name;    //Subtask hman readable name
    Task* _loopFnc; //Funciton to call when looping
    int _runStep;   //numebr of iteration loop has been called
    int _nextTime;  //time when it should call loop again
    subTask* _Next; //Linked list pointer to next subtask in list
};

/* TaskManager Class.  
 *  This class builds a manager to let independent tasks run on an Arduion IDE code base.  Each task time slices, and returns back milliseconds until it wants to trun again
 */
class TaskManager {
public:
  TaskManager(); //Constrctor

  /**
   * Create - Create a new task, this is called during setup phase.  
   * @param {Task} loopFnc - Loop fucntion Task to call when running in loop mode.  Of type int fnc(int).  A Loop function should rturn an integer value indicating milliseconds until the next time it wants to run, and is passed in a count of the numbe rof iterations it has so far run
   * @param {setupFnc} (OPTIONAL) setupFnc - void functon to call if setup is needed for the task, of type void fnc().  
   */
  void Create(const char* name, Task* loopFnc, TaskSetup *setupFnc = NULL);
  
  /**
   * Update - Update all tasks while running in loop
   */
  void Update();
  
 private:
  subTask _subTasks;  //Linked list of subtasks, initial one allocated in class
};

#include "Tasks.h"

TaskManager::TaskManager() {} //Constructor

void TaskManager::Create(const char *name, Task* loopFnc, TaskSetup* setupFnc) { 
  subTask* task = &_subTasks; //Gte head of our linked lists of tasks

  while (task->_loopFnc) { //Look until we find a subtask that doesn't yet has a mandatory loop function defined
    if (!task->_Next) { //If we hit the end of the list, allocate a new one
      task->_Next = new subTask();
    }
    task = task->_Next; //Advance the list to next entry
  }

  //This should be an empty subtask.  Set it's name and loop function
  task->_name = (char *)name;
  task->_loopFnc = loopFnc;

  if (setupFnc) { //If it has a stup function, call it now
    (*setupFnc)(); 
  }

}

void TaskManager::Update() {
  //Update.  Loop through our subtasks, and if any are scheduled to run, run them
  int now = millis();
  int nextRun = now + 2000; //default to at least check every 2 seconds if we should run

  subTask* task = &_subTasks; //Start at the head of our subtasks list

  while (task) {
    if (task->_loopFnc && task->_nextTime <= now) { //valid task says it should run becasue its next time is now or before now
      task->_nextTime = millis() + (*(task->_loopFnc))(task->_runStep); //Execute its loop, getting back a value for when it should run again
      task->_runStep = task->_runStep < 0x0FFFFFFF ? task->_runStep + 1 : 0; //Increment its runStep, ensuring not to overflow
    }
    //Serial.printf("TASK %s next in %d\r\n", task->_name, task->_nextTime - millis());
    nextRun = MIN(task->_nextTime, nextRun); //If its next time is hsorter than our expected next time, remeber it for setting overall delay
    task = task->_Next; //Loop to the next subtask
  }

  //Schedule our delay before allowing loop again
  now = millis();
  int wait = nextRun > now ? nextRun - now : 1;
  //Serial.printf("Delaying %d before next task eval\r\n", wait);
  delay(wait);
}

/* TaskManager 
 * Define tasks that can be time sliced and run independently.  THey are created in Tasks, and then execute their loo funciton when their time is due
 */
TaskManager Tasks;

#endif //BasicTask_h