
#pragma once

struct SRC
{
	CString name;

	short s;
	short r;
	short c;
};

void GetCellNames(IVShapePtr shape, const CString& cell_name_mask, std::vector<SRC>& result);