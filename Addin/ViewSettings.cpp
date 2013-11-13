
#include "stdafx.h"
#include "lib/Utils.h"
#include "ViewSettings.h"

void ViewSettings::AddCellMask(LPCWSTR name)
{
	m_cell_name_masks.push_back(name);
}

void ViewSettings::UpdateCellMask(size_t idx, LPCWSTR text)
{
	m_cell_name_masks[idx] = text;
}

void ViewSettings::RemoveCellMask(size_t idx)
{
	m_cell_name_masks.erase(m_cell_name_masks.begin() + idx);
}

#define ADDIN_KEY	L"Software\\UnmanagedVisio\\ShapeSheetWatch"

void ViewSettings::Save()
{
	CRegKey key;
	if (0 == key.Create(HKEY_CURRENT_USER, ADDIN_KEY))
	{
		CString val_masks = JoinList(m_cell_name_masks, L"|");
		key.SetStringValue(L"Masks", val_masks, REG_SZ);

		Strings column_widths(Column_Count);
		Strings column_hidden(Column_Count);

		for (int i = 0; i < Column_Count; ++i)
		{
			column_widths[i] = FormatString(L"%d", m_column_widths[i]);
			column_hidden[i] = FormatString(L"%d", m_column_hidden[i] != 0);
		}

		CString val_widths = JoinList(column_widths, L"|");
		key.SetStringValue(L"ColumnWidth", val_widths, REG_SZ);

		CString val_hidden = JoinList(column_hidden, L"|");
		key.SetStringValue(L"ColumnHidden", val_hidden, REG_SZ);
	}
}

void ViewSettings::Load()
{
	CRegKey key;
	if (0 == key.Open(HKEY_CURRENT_USER, ADDIN_KEY, KEY_READ))
	{
		WCHAR val[2048] = L"";
		ULONG len = 0;

		len = _countof(val);
		if (0 == key.QueryStringValue(L"Masks", val, &len))
			SplitList(val, L"|", m_cell_name_masks);

		len = _countof(val);
		Strings column_widths;
		if (0 == key.QueryStringValue(L"ColumnWidth", val, &len))
			SplitList(val, L"|", column_widths);

		len = _countof(val);
		Strings column_hidden;
		if (0 == key.QueryStringValue(L"ColumnHidden", val, &len))
			SplitList(val, L"|", column_hidden);

		for (size_t i = 0; i < Column_Count; ++i)
		{
			if (i < column_widths.size())
				m_column_widths[i] = StrToInt(column_widths[i]);

			if (i < column_hidden.size())
				m_column_hidden[i] = StrToInt(column_hidden[i]) != 0;
		}
	}
}

const Strings& ViewSettings::GetCellMasks() const
{
	return m_cell_name_masks;
}

void ViewSettings::SetColumnWidth(int column, int width)
{
	m_column_widths[column] = width;
}

int ViewSettings::GetColumnWidth(int column) const
{
	return m_column_widths[column];
}

void ViewSettings::SetColumnHidden(int column, bool set)
{
	m_column_hidden[column] = set;
}

bool ViewSettings::IsColumnHidden(int column) const
{
	return m_column_hidden[column];
}

ViewSettings::ViewSettings()
	: m_column_widths(Column_Count)
	, m_column_hidden(Column_Count)
{
}

ViewSettings::~ViewSettings()
{
}

CString GetColumnName(int i)
{
	switch (i)
	{
	case Column_Mask:		return L"Mask";
	case Column_Name:		return L"Name";
	case Column_S:			return L"Section";
	case Column_R:			return L"Row";
	case Column_RU:			return L"Row (U)";
	case Column_C:			return L"Column";
	case Column_Formula:	return L"Formula";
	case Column_FormulaU:	return L"Formula (U)";
	case Column_Value:		return L"Result";
	case Column_ValueU:		return L"Result (U)";

	default:	return L"";
	}
}

Column GetColumnFromId(const CString& id)
{
	if (id == L"col_Mask")		return Column_Mask;
	if (id == L"col_Name")		return Column_Name;
	if (id == L"col_Section")	return Column_S;
	if (id == L"col_Row")		return Column_R;
	if (id == L"col_RowU")		return Column_RU;
	if (id == L"col_Column")	return Column_C;
	if (id == L"col_Formula")	return Column_Formula;
	if (id == L"col_FormulaU")	return Column_FormulaU;
	if (id == L"col_Result")	return Column_Value;
	if (id == L"col_ResultU")	return Column_ValueU;

	return Column_Count;
}
