/*
* FILE			 : task_scheduler.c
* PROJECT		 : Task Scheduler Using Stacks and Queues
* PROGRAMMER	 : Mehakpreet Singh, Navjot Singh, Luisa Tatiana Cruz Diaz. 
* FIRST VERSION  : 2024-02-23
* DESCRIPTION	 : This is the Task manager system in which it add the task, process the task, undo the task and show the task list.
*/
// to ignore the warnings
# define _CRT_SECURE_NO_WARNINGS
// to include the libraries
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
// to define the maximum size of the queue
#define MAX_SIZE 20
// to define the structure of the task
typedef struct Task 
{
	char Name[100];
	int PriorityLevel;
	struct Task* next; 
} Task;

// to define the structure of the queue
typedef struct Queue 
{
	Task* Front;
	Task* Back;
} Queue;

//	to define the structure of the stack
typedef struct Stack {
	Task* Data[MAX_SIZE];
	int TopIndex;
}Stack;
//Function pointers of queue
bool IsQueueEmpty(Queue* queue);
Queue* InitializeQueue();
void Enqueue(Queue* queue, Task* task);
void EnqueueFront(Queue* TaskQueue, Task* TaskToUndo);
Task* Dequeue(Queue* queue);
//Function pointers of stack
Stack* InitializeStack();
bool IsStackEmpty(Stack* stack);
bool IsStackFull(Stack* stack);
void Push(Stack* stack, Task* task);
Task* Pop(Stack* stack);
// Function pointers of task
void AddTask(Queue* TaskQueue, char* TaskName, int PriorityLevel);
void Display();
int getIntInput();
void ProcessTask(Queue* TaskQueue, Stack* CompletedTaskStack);
void UndoLastTask(Stack* CompletedTaskStack, Queue* TaskQueue);
void ViewPendingTasks(Queue* TaskQueue);
void ViewCompletedTasks(Stack* CompletedTaskStack);
void freeStack(Stack* stack);
void freeQueue(Queue* queue);

int main(void)
{
	// to define the variables
	char TaskName[100];
	int PriorityLevel;
	int choice;
	// to initialize the queue and stack
	Queue* TaskQueue = InitializeQueue();
	Stack* CompletedTaskStack = InitializeStack();
	do
	{
		Display();
		printf("Enter your choice: ");
		/*scanf_s("%d", &choice);*/
		choice = getIntInput();
		switch (choice)
		{
		case 1:
			//printf("Function to add a task\n");
			printf("Enter Task Name: ");
			fgets(TaskName, sizeof(TaskName), stdin);
			TaskName[strcspn(TaskName, "\n")] = 0;
			printf("Enter Priority Level (1-5): ");
			PriorityLevel = getIntInput();
			//To check input validation
			if (PriorityLevel < 0 || PriorityLevel>5)
			{
				printf("Invalid Priority Level\n");
				break;
			}
			else
			{
				AddTask(TaskQueue,TaskName, PriorityLevel);
			}
			break;
		case 2:
			ProcessTask(TaskQueue, CompletedTaskStack);
			break;
		case 3:
			UndoLastTask(CompletedTaskStack, TaskQueue);
			break;
		case 4:;
			ViewPendingTasks(TaskQueue);
			break;
		case 5:
			ViewCompletedTasks(CompletedTaskStack);
			break;
		case 6:
			freeQueue(TaskQueue); 
			freeStack(CompletedTaskStack); 
			free(TaskQueue);
			free(CompletedTaskStack);
			printf("Exiting the program\n");
			break;
		default:
			printf("Invalid choice!\n");
			break;
		}
	} while (choice != 6);
	return 0;
}
/// <summary>
/// Menu to display the options
/// </summary>
void Display()
{
	printf("\nTask Scheduler\n");
	printf("1. Add a task\n");
	printf("2. Process a task\n");
	printf("3. Undo last completed task\n");
	printf("4. View Pending tasks\n");
	printf("5. View completed tasks\n");
	printf("6. Exit\n");
}
/// <summary>
/// To get the integer input from the user
/// </summary>
/// <returns>int value</returns>
int getIntInput()
{
	char input[50]; 
	int choice;
	while (true)
	{
		// to get the input from the user
		fgets(input, sizeof(input), stdin);
		// to check if the input is a number i.e by phrasing the input
		if (sscanf(input, "%d", &choice) == 1) 
		{
			return choice; 
		}
		// if the input is not a number
		else 
		{
			printf("Invalid input! Please enter a number.\n");
			printf("Enter your choice: ");
		}
	}

}

/// <summary>
/// To view the list of completed tasks
/// </summary>
/// <param name="CompletedTaskStack"> The stack of completed tasks</param>
void ViewCompletedTasks(Stack* CompletedTaskStack)
{
	int count = 1;
	// to check if the stack is empty
	if (IsStackEmpty(CompletedTaskStack))
	{
		printf("\nNo Completed Tasks\n");
		return;
	}
	// to display the completed tasks
	printf("\n-----------------------------------------------------------------------------------\n");
	printf("Completed Tasks\n");
	printf("-------------------------------------\n");
	for (int i = CompletedTaskStack->TopIndex; i >= 0; i--)
	{
		printf("%d. Task Name: %s  (Priority: %d)\n", count, CompletedTaskStack->Data[i]->Name, CompletedTaskStack->Data[i]->PriorityLevel);
	}
	printf("\n-----------------------------------------------------------------------------------\n");
}

/// <summary>
/// To view the list of pending tasks
/// </summary>
/// <param name="TaskQueue"> The queue of pending tasks</param>
void ViewPendingTasks(Queue* TaskQueue)
{
	int count = 1;
	// to check if the queue is empty
	if (IsQueueEmpty(TaskQueue))
	{
		printf("No Pending Tasks\n");
		return;
	}
	// to display the pending tasks
	printf("\n-----------------------------------------------------------------------------------\n");
	Task* CurrentNode = TaskQueue->Front;
	printf("Pending Tasks\n");
	printf("-------------------------------------\n");
	while (CurrentNode != NULL)
	{
		printf("%d. Task Name: %s  (Priority: %d)\n",count, CurrentNode->Name, CurrentNode->PriorityLevel);
		//printf("Priority Level: %d\n", CurrentNode->PriorityLevel);
		CurrentNode = CurrentNode->next;
		count++;
	}
	printf("\n-----------------------------------------------------------------------------------\n");
}

/// <summary>
/// To undo the last completed task
/// </summary>
/// <param name="CompletedTaskStack"> The stack of completed tasks</param>
/// <param name="TaskQueue"> The queue of pending tasks</param>
void UndoLastTask( Stack* CompletedTaskStack, Queue* TaskQueue)
{
	// to check if the stack is empty
	if (IsStackEmpty(CompletedTaskStack))
	{
		printf("ERROR ! No Task to Undo\n");
		return;
	}
	// to pop the last task from the stack
	Task* TaskToUndo = Pop(CompletedTaskStack);
	// to add the task to the front of the queue
	EnqueueFront(TaskQueue, TaskToUndo);
	printf("\nTask Undone Successfully\n");
}

/// <summary>
/// To process the task from the queue to the stack of completed tasks
/// </summary>
/// <param name="TaskQueue"> The queue of pending tasks</param>
/// <param name="CompletedTaskStack"> The stack of completed tasks</param>
void ProcessTask(Queue* TaskQueue, Stack* CompletedTaskStack)
{
	// to check if the queue is empty
	if (IsQueueEmpty(TaskQueue))
	{
		printf("No Task to Process\n");
		return;
	}
	// to dequeue the task from the queue
	Task* TaskToProcess = Dequeue(TaskQueue);
	// to push the task to the stack
	Push(CompletedTaskStack, TaskToProcess);
	printf("Task Processed Successfully\n");
}

/// <summary>
/// To add the task to the queue
/// </summary>
/// <param name="TaskQueue"> The queue of pending tasks</param>
/// <param name="TaskName"> The name of the task</param>
/// <param name="PriorityLevel"> The priority level of the task</param>
void AddTask(Queue* TaskQueue, char* TaskName, int PriorityLevel)
{
	// to create a new task dynamcallay
	Task* NewTask = (Task*)malloc(sizeof(Task));
	// to check if the memory is allocated
	if (NewTask == NULL)
	{
		printf("Insufficient Memory");
		exit(EXIT_FAILURE);
	}
	// to copy the task name
	snprintf(NewTask->Name, sizeof(NewTask->Name), "%s", TaskName);
	NewTask->Name[sizeof(NewTask->Name) - 1] = '\0';
	NewTask->PriorityLevel = PriorityLevel;
	NewTask->next = NULL;
	// to add the task to the queue
	Enqueue(TaskQueue, NewTask);
	printf("Task Added Successfully\n");
}
/// <summary>
/// to initilize the queue
/// </summary>
/// <returns>queue</returns>
Queue* InitializeQueue() 
{
	Queue* queue = (Queue*)malloc(sizeof(Queue));
	// to check if the memory is allocated
	if (queue == NULL) 
	{
		printf("Insufficient Memory");
		exit(EXIT_FAILURE);
	}
	// to set the front and back of the queue to null
	queue->Front = NULL;
	queue->Back = NULL;
	return queue;
}
/// <summary>
/// to check if the queue is empty
/// </summary>
/// <param name="queue"> queue head node</param>
bool IsQueueEmpty(Queue* queue) 
{
	return queue->Front == NULL;
}
/// <summary>
/// to add the task to the queue
/// </summary>
/// <param name="queue">queue head node</param>
/// <param name="task">task address</param>
void Enqueue(Queue* queue, Task* task) 
{
	// to check if the queue is empty
	if (IsQueueEmpty(queue)) 
	{
		// to set the front and back of the queue to the task
		queue->Front = task;
		queue->Back = task;
		return;
	}
	// to set the next of the back of the queue to the task
	queue->Back->next = task;
	queue->Back = task;
}
/// <summary>
/// to add the task to the front of the queue
/// </summary>
/// <param name="TaskQueue">queue head node</param>
/// <param name="TaskToUndo">task address</param>
void EnqueueFront(Queue* TaskQueue, Task* TaskToUndo)
{
	// to check if the queue is empty
	if (IsQueueEmpty(TaskQueue))
	{
		// to set the front and back of the queue to the task
		TaskQueue->Front = TaskToUndo;
		TaskQueue->Back = TaskToUndo;
		return;
	}
	// to set the next of the task to the front of the queue
	TaskToUndo->next = TaskQueue->Front;
	TaskQueue->Front = TaskToUndo;
}

/// <summary>
/// to remove the task from the queue
/// </summary>
/// <param name="queue">queue head node</param>
/// <returns>task address</returns>
Task* Dequeue(Queue* queue) 
{
	// to check if the queue is empty
	if (IsQueueEmpty(queue))
	{
		printf("Queue is Empty");
		return NULL;
	}
	// to set the front of the queue to the task
	Task* taskToDequeue = queue->Front;
	queue->Front = queue->Front->next;
	// to check if the front of the queue is null
	if (queue->Front == NULL) 
	{
		// to set the back of the queue to null
		queue->Back = NULL;
	}
	// to set the next of the task to null
	taskToDequeue->next = NULL; 
	return taskToDequeue;
}

/// <summary>
/// to initilize the stack
/// </summary>
/// <returns>stack</returns>
Stack* InitializeStack() 
{
	// to allocate the memory for the stack
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	// to check if the memory is allocated
	if (stack == NULL)
	{
		printf("Insufficient Memory");
		exit(EXIT_FAILURE);
	}
	// to set the top index of the stack to -1
	stack->TopIndex = -1;
	return stack;
}
/// <summary>
/// to check if the stack is empty
/// </summary>
/// <param name="stack">stack head node</param>
/// <returns>bool value</returns>
bool IsStackEmpty(Stack* stack) 
{
	return stack->TopIndex == -1;
}
/// <summary>
/// to check if the stack is full
/// </summary>
/// <param name="stack">stack head node</param>
/// <returns>bool value</returns>
bool IsStackFull(Stack* stack) 
{
	return stack->TopIndex == MAX_SIZE - 1;
}
/// <summary>
/// to add the task to the stack
/// </summary>
/// <param name="stack">stack head node</param>
/// <param name="task">task address</param>
void Push(Stack* stack, Task* task) 
{
	// to check if the stack is full
	if (IsStackFull(stack)) 
	{
		printf("Cannot Insert. Stack Overflow");
		return;
	}
	// to set the next of the task to the top of the stack
	stack->Data[++stack->TopIndex] = task;
}
/// <summary>
/// to remove the task from the stack
/// </summary>
/// <param name="stack">stack head node</param>
/// <returns>task address</returns>
Task* Pop(Stack* stack) 
{
	// to check if the stack is empty
	if (IsStackEmpty(stack)) 
	{
		printf("Stack is Empty\n");
		return NULL;
	}
	// to return the task from the top of the stack
	return stack->Data[stack->TopIndex--];
}
/// <summary>
/// to free the memory allocated for the stack
/// </summary>
/// <param name="stack">stack head node</param>
void freeStack(Stack* stack)
{
	// to check if the stack is null
	if (stack == NULL)
	{
		return; // Nothing to free if the stack is NULL
	}
	// to free the memory allocated for the stack
	while (!IsStackEmpty(stack))
	{
		// to pop the task from the stack
		Task* task = Pop(stack);
		if (task != NULL)
		{
			free(task); // Free the task if Pop returns a valid pointer
		}
	}
}
/// <summary>
/// to free the memory allocated for the queue
/// </summary>
/// <param name="queue">queue head node</param>
void freeQueue(Queue* queue)
{
	// to check if the queue is null
	if (queue == NULL)
	{
		return; // Nothing to free if the queue is NULL
	}
	// to free the memory allocated for the queue
	Task* current = queue->Front;
	// to loop through the queue
	while (current != NULL)
	{
		// to set the next of the current task to the current task
		Task* temp = current;
		current = current->next;
		free(temp);
	}
	queue->Front = queue->Back = NULL; // Reset the queue pointers
}