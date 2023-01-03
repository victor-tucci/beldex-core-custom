#include "crypto/cn_heavy_hash.hpp"


	static cn_heavy_hash_v1 cn_heavy_hash::make_borrowed(cn_heavy_hash_v2& t)
	{
		return {t.lpad.as_void(), t.spad.as_void()};
	}


