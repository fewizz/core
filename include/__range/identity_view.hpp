#pragma once

namespace __range {

	template<typename Range>
	class identity_view {
		Range range_;
	public:
		identity_view(Range&& range) : range_{ forward<Range>(range) } {}

		

	};

} // __range