
/*
    Visual diagram of program workflow. There are two types of threads in this workflow,
    the consumer threads and the producer threads. The producer threads read data from a file
    and push them in a queue, from where the consumer threads can access, retrieve, and process.
    Producer-Consumer Model:
        Producer Thread: This thread will handle the read_data function. It reads data from the I/O source and places it into a shared buffer.
        Consumer Thread: This thread will handle the process_data function. It retrieves data from the shared buffer and processes it.

    Producer Thread:
    1. Read Data
    2. Lock Mutex ----+
    3. Push Data      |
    4. Unlock Mutex <-+
    5. Notify Consumer (cv.notify_one())

    Consumer Thread:
    1. Lock Mutex ----+
    2. Wait (cv.wait(lock, predicate))
    - Release Mutex
    - Sleep
    3. Wakes Up
    - Reacquire Mutex
    4. Check Predicate
    - If true, proceed
    5. Retrieve Data
    6. Unlock Mutex <-+
    7. Process Data
*/

#include <iostream>
#include <fstream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

class DataProcessor {

    public:
        DataProcessor(std::ifstream& inFile, size_t max_queue_size)
            : file(inFile), done(false), maxQueueSize(max_queue_size) {}

        // Function to read data
        void read_data() {
            std::string line;
            while (std::getline(file, line)) {
                if (line == "---") { // Dataset delimiter
                    break;
                }

                // Lock mutex before accessing the queue
                std::unique_lock<std::mutex> lock(mtx);

                // Wait until queue has space
                space_available.wait(lock, [this]() {
                    return dataQueue.size() < maxQueueSize;
                });

                // Push data into the queue
                dataQueue.push(line);

                // Notify one consumer thread that data is available
                data_available.notify_one();
            }

            // Set done flag and notify all consumer threads
            {
                std::unique_lock<std::mutex> lock(mtx);
                done = true;
            }
            data_available.notify_all();
        }

        // Function to process data
        void process_data() {
            while (true) {
                std::string data;
                // Lock mutex before accessing the queue
                std::unique_lock<std::mutex> lock(mtx);

                // Wait until data is available or done is true
                data_available.wait(lock, [this]() {
                    return !dataQueue.empty() || done;
                });

                if (!dataQueue.empty()) {
                    // Retrieve data from the queue
                    data = dataQueue.front();
                    dataQueue.pop();

                    // Notify producer thread that space is available
                    space_available.notify_one();
                } else if (done) {
                    // No more data will arrive
                    break;
                }

                // Unlock mutex before processing data
                lock.unlock();

                if (!data.empty()) {
                    // Process the data outside the lock
                    process_line(data);
                }
            }
        }

        // Run the data processing with overlapped I/O and computation
        void run(int num_consumers) {
            // start a producer thread that pushes constantly data in the shared queue (as lons as the maxQueueSize is not exceeded).
            std::thread reader(&DataProcessor::read_data, this);

            // Start multiple processing threads that consume data on demand.
            std::vector<std::thread> processors;
            for (int i = 0; i < num_consumers; ++i) {
                processors.emplace_back(&DataProcessor::process_data, this);
            }

            reader.join();
            for (auto& processor : processors)
                processor.join();
        }

    private:
        std::ifstream& file;                     // Reference to the input file stream
        std::queue<std::string> dataQueue;       // Queue accessible from producer and consumer threads
        std::mutex mtx;                          // Guards access to the shared queue
        std::condition_variable data_available;  // Notifies that data is available
        std::condition_variable space_available; // Notifies that space is available in the queue
        bool done;                               // Flag that becomes true when all data have been read
        size_t maxQueueSize;                     // Limits the max number of data inside queue

        // Simulate processing a line of data
        void process_line(const std::string& line) {
            // Replace this with actual processing logic
            std::cout << "Processed: " << line << std::endl;
        }
};

int main() {
    // Open the input file
    std::ifstream inFile("data.txt");
    if (!inFile.is_open()) {
        std::cerr << "Unable to open file." << std::endl;
        return 1;
    }

    while (true) {

        // Check for end of file before creating a new DataProcessor
        if (inFile.eof()) {
            break;
        }

        // Create a DataProcessor instance for each dataset with max queue size 4
        DataProcessor processor(inFile, 4); // Set maxQueueSize to 4
        processor.run(3);
    }

    inFile.close();
    return 0;
}



/*
How the Lock Protects the Queue?
--------------------------------
1. Mutual Exclusion with Mutex
    Mutex (std::mutex mtx):
        A mutual exclusion primitive that prevents multiple threads from accessing a shared resource simultaneously.
    Locking the Mutex:
        A thread must acquire the mutex lock before accessing the shared resource.
        Only one thread can hold the mutex at any given time.

2. Acquiring the Lock
    std::unique_lock<std::mutex> lock(mtx);:
        Locks the mutex mtx upon construction of the lock object.
        Ensures that the thread has exclusive access to the protected code block.
    Scope-Based Locking:
        The lock remains active for the duration of the scope ({}) in which it is declared.
        Upon exiting the scope, the lock is automatically released (RAII idiom).





What Are Condition Variables?
-----------------------------
A condition variable is a synchronization primitive provided by threading libraries (like the C++ Standard Library) 
that allows threads to wait for certain conditions to be met before proceeding. They are used in conjunction with 
a mutex to control access to shared resources and to coordinate the execution of threads.

Key Points:
    Purpose: Enable threads to wait efficiently (without busy-waiting) for some condition to become true.
    Usage: Typically used in producer-consumer scenarios where one thread (producer) signals another 
    thread (consumer) that a condition has changed.
    Mutex Association: A condition variable is always used alongside a mutex to protect shared data.





Why Do We Need Condition Variables?
-----------------------------------
In multithreaded programs, threads often need to coordinate their actions based on the state of shared data. 
For example, a consumer thread may need to wait until a producer thread has produced data. Without condition variables, 
the consumer thread would have to continuously check (poll) the shared data in a loop, which is inefficient and 
wastes CPU resources—a practice known as busy-waiting. Condition variables solve this problem by allowing a thread 
to sleep until another thread notifies it that the condition has changed. This results in efficient synchronization 
and better CPU utilization.





How Do Condition Variables Work?
--------------------------------
Here's a step-by-step explanation of how condition variables function:

    1. Mutex Locking:
        A thread must hold a lock on a mutex before it can wait on a condition variable.
        This ensures exclusive access to the shared data while the condition is checked.

    2. Wait Operation:
        The thread calls wait() on the condition variable, passing the mutex lock.
        The wait() function:
            Atomically releases the mutex lock.
            Puts the thread to sleep until it is notified.

    3. Notification:
        Another thread (usually the one modifying the shared data) calls notify_one() or notify_all() on the 
        condition variable to wake up waiting threads.

    4. Reacquiring the Mutex:
        When the waiting thread is notified, it wakes up and reacquires the mutex lock before proceeding.
        This ensures the shared data is protected when the condition is rechecked.

    5. Condition Checking:
        The thread rechecks the condition (often in a loop) to decide whether to proceed or wait again.
        This is necessary because:
            Spurious Wake-ups: Threads can sometimes wake up without a notification.
            Race Conditions: The condition might have changed again before the thread acquired the mutex.
*/