#pragma once
#include <xhash>

#include <unordered_map>

namespace Cyclope {

	class CYCLOPE_API UUID {

	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_uuid; }
	private:
		uint64_t m_uuid;

	};

}

namespace std {

	template<>
	struct CYCLOPE_API hash<Cyclope::UUID> {
		std::size_t operator()(const Cyclope::UUID& uuid) const {
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};

}