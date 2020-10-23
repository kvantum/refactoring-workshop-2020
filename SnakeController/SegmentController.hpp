#pragma once

#include <list>
#include <memory>
#include <functional>

#include "IEventHandler.hpp"
#include "SnakeInterface.hpp"

class SegmentController : public IEventHandler
{
public:

  struct Segment
  {
      int x;
      int y;
  };

  bool isSegmentAtPosition(int x, int y) const;
  Segment calculateNewHead() const;
  void updateSegmentsIfSuccessfullMove(Segment const& newHead);
  void addHeadSegment(Segment const& newHead);
  void removeTailSegmentIfNotScored(Segment const& newHead);
  void removeTailSegment();
};
