#include "../src/containers/Queue.h"
#include "../src/containers/PriorityQueue.h"
#include <gtest/gtest.h>
TEST(QueueTest, DefaultConstructor) {
    Queue<int> q;
    EXPECT_TRUE(q.empty());
}

TEST(QueueTest, EnqueueAndFront) {
    Queue<int> q;
    q.push(10);
    q.push(20);
    q.push(30);
    EXPECT_EQ(q.front(), 10);
    EXPECT_EQ(q.getSize(), 3);
}


TEST(QueueTest, Dequeue) {
    Queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);

    q.dequeue();
    EXPECT_EQ(q.front(), 2);

    q.dequeue();
    EXPECT_EQ(q.front(), 3);

    q.dequeue();
    EXPECT_TRUE(q.empty());
}

TEST(QueueTest, DequeueEmptyQueue) {
    Queue<int> q;
    EXPECT_THROW(q.dequeue(), std::runtime_error);
}

TEST(QueueTest, FrontEmptyQueue) {
    Queue<int> q;
    EXPECT_THROW(q.front(), std::runtime_error);
}

TEST(QueueTest, CopyConstructor) {
    Queue<int> q1;
    q1.push(5);
    q1.push(10);

    Queue<int> q2(q1);
    EXPECT_EQ(q2.front(), 5);
    q2.dequeue();
    EXPECT_EQ(q2.front(), 10);
}

TEST(QueueTest, CopyAssignment) {
    Queue<int> q1;
    q1.push(100);
    q1.push(200);

    Queue<int> q2;
    q2 = q1;

    EXPECT_EQ(q2.front(), 100);
    q2.dequeue();
    EXPECT_EQ(q2.front(), 200);
}

TEST(QueueTest, MoveConstructor) {
    Queue<int> q1;
    q1.push(42);

    Queue<int> q2(std::move(q1));
    EXPECT_EQ(q2.front(), 42);
    EXPECT_TRUE(q1.empty());
}

TEST(QueueTest, MoveAssignment) {
    Queue<int> q1;
    q1.push(7);
    q1.push(14);

    Queue<int> q2;
    q2 = std::move(q1);

    EXPECT_EQ(q2.front(), 7);
    q2.dequeue();
    EXPECT_EQ(q2.front(), 14);
    EXPECT_TRUE(q1.empty());
}


TEST(QueueTest, Reverse) {
    Queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);

    Queue<int> reversed = q.reverse();
    EXPECT_EQ(reversed.front(), 3);
    reversed.dequeue();
    EXPECT_EQ(reversed.front(), 2);
    reversed.dequeue();
    EXPECT_EQ(reversed.front(), 1);
}

TEST(QueueTest, IsEmpty) {
    Queue<int> q;
    EXPECT_TRUE(q.empty());
    q.push(5);
    EXPECT_FALSE(q.empty());
    q.dequeue();
    EXPECT_TRUE(q.empty());
}

TEST(QueueTest, GetSize) {
    Queue<int> q;
    EXPECT_EQ(q.getSize(), 0);
    q.push(1);
    q.push(2);
    EXPECT_EQ(q.getSize(), 2);
}

TEST(PriorityQueueTest, DefaultConstructor) {
    PriorityQueue<int> pq;
    EXPECT_TRUE(pq.empty());
}

TEST(PriorityQueueTest, PushAndTop) {
    PriorityQueue<int> pq;
    pq.push(10, 1);
    pq.push(20, 2);
    pq.push(15, 3);
    pq.pop();
    EXPECT_EQ(pq.top().nodeItem, 20);
}

TEST(PriorityQueueTest, PopEmptyQueue) {
    PriorityQueue<int> pq;
    EXPECT_THROW(pq.pop(), std::runtime_error);
}


TEST(PriorityQueueTest, ExtractMin) {
    PriorityQueue<int> pq;
    pq.push(10, 3);
    pq.push(5, 1);
    pq.push(7, 2);

    EXPECT_EQ(pq.extractMin(), 5);
    EXPECT_EQ(pq.extractMin(), 7);
    EXPECT_EQ(pq.extractMin(), 10);
}


TEST(PriorityQueueTest, ChangePriority) {
    PriorityQueue<int> pq;
    pq.push(10, 3);
    pq.push(5, 1);
    pq.push(7, 2);

    pq.changePriority(1, 4);
    EXPECT_EQ(pq.top().nodeItem, 7);
}

TEST(PriorityQueueTest, GetSize) {
    PriorityQueue<int> pq;
    pq.push(10, 3);
    pq.push(5, 1);

    EXPECT_EQ(pq.getSize(), 2);
}


TEST(PriorityQueueTest, Clear) {
    PriorityQueue<int> pq;
    pq.push(10, 3);
    pq.push(5, 1);

    pq.clear();
    EXPECT_TRUE(pq.empty());
}

TEST(PriorityQueueTest, Merge) {
    PriorityQueue<int> pq1;
    PriorityQueue<int> pq2;

    pq1.push(10, 2);
    pq1.push(20, 1);

    pq2.push(5, 3);
    pq2.push(15, 4);

    pq1.merge(pq2);

    EXPECT_EQ(pq1.getSize(), 4);
}
