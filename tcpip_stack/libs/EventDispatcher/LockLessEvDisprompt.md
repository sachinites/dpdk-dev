# Project: Design and Implement a Lockless Event Dispatcher from Scratch

## Role

You are an expert C/C++ systems programmer specializing in:
- Lock-free and lockless concurrent data structures
- Producer-consumer architectures
- Event-driven programming
- Operating system scheduling concepts
- DPDK and high-performance networking
- Multithreaded systems programming

## Objective

Design and implement a complete **Lockless Event Dispatcher library** in C++17 from scratch.

The objective is to build a reusable event-loop scheduler that solves the multi-producer/single-consumer problem efficiently and supports multiple independent packet queues, callback-driven processing, and priority-based scheduling.

The library must be suitable for integration into a networking stack.

## Available Input

I will provide the following file:

`PubLockLessEvDis.h`

This header defines the public API exposed to applications using the library.

**Treat this header as the authoritative functional contract.**

Applications must be able to use the library exclusively through this public header without knowing its internal implementation.

You do NOT have access to an existing implementation of this library.

You must independently design and implement the entire library from scratch.

Do not assume that internal structures, algorithms, or implementation details already exist.

## Functional Requirements

### 1. Event Dispatcher

Implement an event dispatcher that:

- Can be created and destroyed dynamically.
- Runs its event-processing loop in a dedicated thread.
- Supports pinning its consumer thread to a specified CPU core.
- Supports multiple independent packet queues.
- Processes queues according to their configured priorities.
- Supports graceful stopping of its processing thread.
- Allows applications to determine whether their callback should cooperatively suspend processing.
- Supports querying the currently executing packet queue.
- Supports debugging and inspection of pending scheduler work.

### 2. Producer-Consumer Architecture

The dispatcher must support:

- Multiple concurrent producer threads.
- One dedicated consumer thread.
- Producers independently submitting packets to different packet queues.
- The consumer processing packets through registered callbacks.
- Efficient notification and scheduling of packet queues containing pending work.

The consumer must not need to continuously scan all packet queues to discover pending work.

### 3. Packet Queues

Each packet queue must:

- Be independently created and associated with an event dispatcher.
- Have a callback responsible for processing packets.
- Support configurable scheduling priority.
- Support bounded packet buffering.
- Support packet enqueueing from multiple producer threads.
- Track packet counts and dropped packets.
- Support optional packet cleanup callbacks.
- Support queue destruction and resource reclamation.

Some queues may be passive queues without a callback. Such queues should still support packet storage and retrieval through the public API.

### 4. Priority-Based Scheduling

The scheduler must support multiple priority levels.

The priority ordering and meanings are defined in the supplied public header.

Implement a scheduler that:

- Prefers higher-priority queues over lower-priority queues.
- Efficiently schedules queues that become ready.
- Avoids repeatedly scheduling queues that are already waiting for execution.
- Correctly reschedules queues that still contain pending packets after processing.
- Prevents lower-priority queues from being permanently starved.
- Supports cooperative callback suspension.

### 5. Callback Execution Model

Callbacks must:

- Execute exclusively in the dispatcher consumer thread.
- Process packets using the packet retrieval API exposed in the public header.
- Be able to process multiple packets during a single callback invocation.
- Cooperatively return control to the scheduler when required.
- Be able to determine whether the scheduler requests suspension.

The dispatcher must preserve the callback execution semantics defined in the public header.

## Architectural Design

Independently design the internal architecture required to implement the public API.

You must determine:

- Internal data structures for dispatcher and packet queues.
- How packets are stored and retrieved.
- How producer threads safely communicate with the consumer.
- How ready queues are represented.
- How packet queues are scheduled.
- How priority scheduling is implemented.
- How the consumer discovers ready work.
- How queues transition between empty, ready, executing, and pending states.
- How synchronization and memory ordering are handled.
- How packet ownership is transferred between producers and consumers.
- How queue destruction and dispatcher shutdown are made safe.
- How concurrent enqueue operations interact with queue scheduling.

You may use appropriate DPDK primitives such as `rte_ring` if they are suitable.

However, **do not assume that a particular internal algorithm is mandatory**.

Evaluate alternative designs where appropriate and select an implementation based on correctness, efficiency, maintainability, and concurrency guarantees.

## Lockless Implementation Requirement

The packet submission and scheduling architecture must be designed to minimize synchronization overhead and avoid conventional mutex-based producer-consumer synchronization in the packet fast path.

The implementation should be lockless wherever reasonably achievable.

If some lifecycle or control-plane operations require synchronization, explain why and ensure they do not unnecessarily affect the packet-processing fast path.

Do not simply label an implementation lockless without explaining its synchronization guarantees.

## Concurrency and Correctness

Carefully reason about the following scenarios:

- Multiple producers enqueueing packets simultaneously.
- A producer enqueueing into an empty queue.
- A producer enqueueing while the consumer is processing the same queue.
- A queue becoming empty while producers are concurrently enqueueing.
- A queue becoming ready while it is already scheduled.
- A queue remaining nonempty after callback execution.
- A queue being destroyed while producers are attempting to enqueue.
- A dispatcher being stopped while queues contain pending packets.
- Packet cleanup during queue destruction.
- Memory reclamation and prevention of use-after-free.
- Prevention of duplicate scheduling and lost wakeups.
- Correct packet ownership during successful and unsuccessful enqueue operations.

Document the concurrency guarantees and assumptions made by the implementation.

## Implementation Requirements

Implement the library in a clean and modular manner.

The public API must remain unchanged unless an issue is identified that genuinely prevents implementation.

Use C++17.

Separate public interfaces from private implementation details.

Do not expose internal structures to applications.

Ensure the implementation can be compiled and linked independently as a reusable library.

## Testing

Develop a comprehensive test suite covering:

1. Basic dispatcher lifecycle.
2. Basic packet enqueue and callback processing.
3. Multiple concurrent producers.
4. Multiple packet queues.
5. Priority scheduling.
6. Queue capacity and packet drops.
7. Passive packet queues.
8. Callback suspension and cooperative preemption.
9. Queue destruction with pending packets.
10. Dispatcher shutdown with pending packets.
11. Packet cleanup callbacks.
12. Concurrent producer and consumer activity.
13. Stress testing with large packet volumes.
14. Race-condition testing.
15. Memory safety and resource reclamation.

Use sanitizers and concurrency testing tools where available.

## Documentation

Provide documentation explaining:

- Overall architecture.
- Producer-consumer execution model.
- Internal data structures and their rationale.
- Scheduling algorithm.
- Priority handling.
- Packet ownership.
- Thread-safety guarantees.
- Queue lifecycle.
- Dispatcher lifecycle.
- Lockless design and synchronization.
- Usage examples.
- Compilation and execution of tests.

## Expected Deliverables

Produce:

1. Complete C++ implementation.
2. Private internal header files.
3. Public API compatibility with `PubLockLessEvDis.h`.
4. Comprehensive test programs.
5. Example application demonstrating library usage.
6. Build scripts or build-system integration.
7. Architecture and design documentation.
8. Concurrency and correctness analysis.

## Working Method

Proceed incrementally:

1. Analyze the public API and derive functional requirements.
2. Identify ambiguities and document assumptions.
3. Propose an architecture and explain its design decisions.
4. Implement the core producer-consumer mechanism.
5. Implement packet queue scheduling and priorities.
6. Implement lifecycle and cleanup handling.
7. Develop tests and validate the implementation.
8. Optimize the packet-processing fast path.
9. Complete documentation.

Do not skip architectural reasoning and directly jump into writing code.

The primary objective is to produce a **correct, reusable, efficient, and independently designed lockless event dispatcher**, not merely code that compiles.