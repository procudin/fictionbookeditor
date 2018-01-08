// FBDoc.h: interface for the FBDoc class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "FBEView.h"

namespace FB // put all FB2 related stuff into its own namespace
{
// an fb2 document in-memory representation
class Doc
{
public:
	// document encoding
	CString m_encoding;

	// owner frame
	HWND m_frame;

	// document text is stored here
	CFBEView m_body;

	// filename
	CString m_filename;
	bool m_namevalid;
	bstr_t m_save_marker;

	static bool m_fast_mode;

	MSHTML::IHTMLElementPtr m_saved_element;

	// construction and destruction
	Doc(HWND hWndFrame);
	~Doc();

	void SaveSelectedPos();
	void DeleteSaveMarker();
	long GetSavedPos(bstr_t & dom, bool deleteMarker = true);

	// loading and savingaa
	void CreateBlank(HWND hWndParent);
	bool Load(HWND hWndParent, const CString & filename);
	bool LoadFromHTML(HWND hWndParent, const CString & filename);
	MSXML2::IXMLDOMDocument2Ptr CreateDOM(const CString & encoding);
	HRESULT InvokeFunc(BSTR FuncName, CComVariant * params, int count, CComVariant & vtResult);
	void ShowDescription(bool Show);
	void RunScript(BSTR filePath);
	VARIANT_BOOL Doc::CheckScript(BSTR filePath);

	bool Save(const CString & filename);

	bool Validate(int & errline, int & errcol);

	bool SetXMLAndValidate(HWND sci, bool fValidateOnly, int & errline, int & errcol);
	bool TextToXML(BSTR text, MSXML2::IXMLDOMDocument2Ptr * xml);

	bool Save();

	// changes
	bool DocChanged();

	// added by SeNS
	void AdvanceDocVersion(int delta);
	void MarkSavePoint();
	void ResetSavePoint();
	void MarkDocCP();
	bool DocRelChanged();

	// IDs
	void BinIDsToComboBox(CComboBox & box);
	void ParaIDsToComboBox(CComboBox & box);

	// binary objects
	BSTR PrepareDefaultId(const CString & filename);
	void AddBinary(const CString & filename);

	// config
	void ApplyConfChanges();

	// active document table
	static Doc * LocateDocument(const wchar_t * id);
	static Doc * m_active_doc;

	// binary objects
	bool GetBinary(const wchar_t * id, _variant_t & vt);

	// Word lists
	struct Word
	{
		CString word;
		CString replacement;
		int count;
		int flags;

		Word()
		    : count(0), flags(0)
		{
		}
		Word(CString & word, int count)
		    : word(word), count(count), flags(0)
		{
		}
	};

	enum
	{
		GW_INCLUDE_HYPHENS = 1,
		GW_HYPHENS_ONLY = 2,
		GW_SORT_BY_COUNT = 4
	};

	void GetWordList(int flags, CSimpleArray<Word> & words, CString tagName);

private:
	long m_body_ver;
	long m_body_cp;

	// saving support
	bool SaveToFile(const CString & filename, bool fValidateOnly = false, int * errline = NULL, int * errcol = NULL);
	MSXML2::IXMLDOMDocument2Ptr CreateDOMImp(const CString & encoding);

	// loading support
	void TransformXML(MSXML2::IXSLTemplatePtr tp, MSXML2::IXMLDOMDocument2Ptr doc, CFBEView & dest);
	LPCWSTR MyID() const;
	LPCWSTR MyURL(_In_z_ LPCWSTR pszPart) const;
	static CSimpleMap<Doc *, Doc *> m_active_docs;

public:
	MSHTML::IHTMLDOMNodePtr MoveNode(MSHTML::IHTMLDOMNodePtr from, MSHTML::IHTMLDOMNodePtr to, MSHTML::IHTMLDOMNodePtr insertBefore);
	void SetFastMode(bool fast);
	bool GetFastMode() const;
	int GetSelectedPos();

	CString GetOpenFileName() const;

private:
	void FastMode();
};

} // namespace FB
