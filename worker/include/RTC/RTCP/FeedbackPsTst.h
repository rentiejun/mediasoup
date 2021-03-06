#ifndef MS_RTC_RTCP_FEEDBACK_TST_H
#define MS_RTC_RTCP_FEEDBACK_TST_H

#include "common.h"
#include "RTC/RTCP/FeedbackPs.h"

/* RFC 5104
 * Temporal-Spatial Trade-off Request (TSTR)
 * Temporal-Spatial Trade-off Notification (TSTN)

    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
0  |                              SSRC                             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
4  |  Seq nr.      |
5                  |  Reserved                           | Index   |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

namespace RTC { namespace RTCP
{
	template<typename T> class TstItem
		: public FeedbackItem
	{
	private:
		struct Header
		{
			uint32_t ssrc;
			uint32_t sequence_number:8;
			uint32_t reserved:19;
			uint32_t index:5;
		};

	public:
		static const FeedbackPs::MessageType MessageType;

	public:
		static TstItem* Parse(const uint8_t* data, size_t len);

	public:
		TstItem(Header* header);
		TstItem(TstItem* item);
		TstItem(uint32_t ssrc, uint8_t sequenceNumber, uint8_t index);

		// Virtual methods inherited from FeedbackItem
		void Dump() override;
		size_t Serialize(uint8_t* data) override;
		size_t GetSize() override;

		uint32_t GetSsrc();
		uint8_t GetSequenceNumber();
		uint8_t GetIndex();

	private:
		// Passed by argument.
		Header* header = nullptr;
	};

	class Tstr {};
	class Tstn {};

	// Tst classes declaration
	typedef TstItem<Tstr> TstrItem;
	typedef TstItem<Tstn> TstnItem;

	// Tst packets declaration
	typedef FeedbackPsItemPacket<TstrItem> FeedbackPsTstrPacket;
	typedef FeedbackPsItemPacket<TstnItem> FeedbackPsTstnPacket;

	/* TstItem inline instance methods */

	template <typename T>
	TstItem<T>::TstItem(Header* header):
		header(header)
	{
	}

	template <typename T>
	TstItem<T>::TstItem(TstItem* item) :
		header(item->header)
	{
	}

	template <typename T>
	size_t TstItem<T>::GetSize()
	{
		return sizeof(Header);
	}

	template <typename T>
	uint32_t TstItem<T>::GetSsrc()
	{
		return (uint32_t)ntohl(this->header->ssrc);
	}

	template <typename T>
	uint8_t TstItem<T>::GetSequenceNumber()
	{
		return (uint8_t)this->header->sequence_number;
	}

	template <typename T>
	uint8_t TstItem<T>::GetIndex()
	{
		return (uint8_t)ntohl(this->header->index);
	}

} } // RTP::RTCP

#endif
