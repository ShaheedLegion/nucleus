// Copyright (c) 2015, Tiaan Louw
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
// OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#ifndef NUCLEUS_MESSAGE_LOOP_MESSAGE_LOOP_H_
#define NUCLEUS_MESSAGE_LOOP_MESSAGE_LOOP_H_

#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "nucleus/macros.h"

namespace nu {

class MessageLoop {
public:
  using Task = std::function<void()>;

  MessageLoop();
  ~MessageLoop();

  // Add a task to the queue to be performed.
  void postTask(const Task& callback);

  // Run the message loop.
  void run();

  // Run the message loop until we go idle.
  void runUntilIdle();

  // Request that the message loop quit as soon as possible.
  void requestQuit();

private:
  // Move work from the incoming work queue to the work queue.
  void reloadWorkQueue();

  // Perform any immediate tasks in the work queue.
  bool doWork();

  // Perform any tasks when this message loop goes idle.
  bool doIdleWork();

  // Post a task to the queue that will set the loop to exit as soon as
  // possible.
  void quitInternal();

  // This will be set to true if the message loop should quit as soon as
  // possible.
  bool m_shouldQuit{false};

  // This is set by runUntilIdle so that we set m_isRunning to false as soon as
  // we hit the first idle.
  bool m_quitWhenIdle{false};

  // The queue where incoming tasks will be added.
  std::queue<Task> m_incomingQueue;

  // The queue of tasks that we need to perform.
  std::queue<Task> m_workQueue;

  // The condition that is notified when we have work to do.
  std::condition_variable m_workIsAvailable;

  DISALLOW_COPY_AND_ASSIGN(MessageLoop);
};

}  // namespace nu

#endif  // NUCLEUS_MESSAGE_LOOP_MESSAGE_LOOP_H_