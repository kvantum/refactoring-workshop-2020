#include "SnakeController.hpp"

#include <algorithm>
#include <sstream>

#include "EventT.hpp"
#include "IPort.hpp"

namespace Snake
{
SnakeController::Segment Segment::calculateNewHead() const
    {
        Segment const& currentHead = m_segments.front();

        Segment newHead;
        newHead.x = currentHead.x + (Controller::isHorizontal(m_currentDirection) ? Controller::isPositive(m_currentDirection) ? 1 : -1 : 0);
        newHead.y = currentHead.y + (isVertical(m_currentDirection) ? isPositive(m_currentDirection) ? 1 : -1 : 0);

        return newHead;
    }

    void SegmentController::removeTailSegment()
    {
        auto tail = m_segments.back();

        DisplayInd l_evt;
        l_evt.x = tail.x;
        l_evt.y = tail.y;
        l_evt.value = Cell_FREE;
        m_displayPort.send(std::make_unique<EventT<DisplayInd>>(l_evt));

        m_segments.pop_back();
    }

    void SegmentController::addHeadSegment(Segment const& newHead)
    {
        m_segments.push_front(newHead);

        DisplayInd placeNewHead;
        placeNewHead.x = newHead.x;
        placeNewHead.y = newHead.y;
        placeNewHead.value = Cell_SNAKE;

        m_displayPort.send(std::make_unique<EventT<DisplayInd>>(placeNewHead));
    }

    void SegmentController::removeTailSegmentIfNotScored(Segment const& newHead)
    {
        if (std::make_pair(newHead.x, newHead.y) == m_foodPosition) {
            m_scorePort.send(std::make_unique<EventT<ScoreInd>>());
            m_foodPort.send(std::make_unique<EventT<FoodReq>>());
        } else {
            removeTailSegment();
        }
    }

    bool SegmentController::isSegmentAtPosition(int x, int y) const
    {
        return m_segments.end() !=  std::find_if(m_segments.cbegin(), m_segments.cend(),
            [x, y](auto const& segment){ return segment.x == x and segment.y == y; });
    }


    void SegmentController::updateSegmentsIfSuccessfullMove(Segment const& newHead)
    {
        if (isSegmentAtPosition(newHead.x, newHead.y) or isPositionOutsideMap(newHead.x, newHead.y)) {
            m_scorePort.send(std::make_unique<EventT<LooseInd>>());
        } else {
            addHeadSegment(newHead);
            removeTailSegmentIfNotScored(newHead);
        }
    }
  }
}
