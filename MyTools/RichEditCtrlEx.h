#pragma once
#include "afxcmn.h"



/////////////////////////////////////////////////////////////////////////////
// Name:        RICHEDITCTRLEX.H
// Purpose:     CRichEditCtrlEx
// Author:      yfq
// Modified by: 
// Created:     2010/12/24 11:01
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

class CRichEditCtrlEx :	public CRichEditCtrl
{
	DECLARE_DYNCREATE(CRichEditCtrlEx)
public:
	CRichEditCtrlEx(void);
	~CRichEditCtrlEx(void);
	
	void Initlize(int i);
	/**
	* 最后追加一行
	*
	* @param text -[in] -要追加的文本
	*
	* @param newline -[in] -是否换行
	*
	* @param link	 -[in] -是否带超链接
	*
	* @param nLinkBegin -[in] -超链接起始位置(索引从0开始)
	*
	* @param nLinkCount -[in] -超链接字符数
	*
	* @return -[NONE]
	*/
	void Append(LPCTSTR text,bool newline = false,bool link = false,int nLinkBegin = 0,int nLinkCount = 0);
	
	/**
	 * 获得文本
	 *
	 * @param nBegin -[in] -开始位置
	 *
	 * @param nEnd -[in] -结束位置
	 *
	 * @param text -[out] -得到的文本
	 *
	 * @return -[NONE]
	 */
	void GetText(int nBegin,int nEnd,CString& text);
	
	/**
	 * 新增文字时是否滚动到底部
	 *
	 * @param bottom -[in] -否滚动到底部
	 *
	 * @return -[NONE]
	 */
	void SetScrollBottom(bool bottom=true);
	void StopScroll();

	/**
	 * 设置字体
	 *
	 * @param nBegin -[in] -开始位置
	 *
	 * @param nEnd -[in] -结束位置
	 *
	 * @param bold -[in] -粗体
	 *
	 * @param italic -[in] -斜体
	 *
	 * @param underline -[in] -下划线
	 *
	 * @param color -[in] -颜色
	 *
	 * @param size -[in] -文字大小
	 *
	 * @return -[NONE]
	 */
	void SetTextFont(int nBegin,int nEnd,bool bold,bool italic,bool underline,COLORREF color=0,int size=0);
	
	/**
	 * 为选中的文本添加超链接
	 *
	 * @return -[NONE]
	 */
	void AddLink();

	/**
	 * 取消选中文本添的超链接
	 *
	 * @return -[NONE]
	 */
	void CancelLink();

	/**
	* 校验URL是否合法文件路径，依据为rfc2396
	* 这个函数只做粗略的校验，不完全符合RFC2369
	* 仅支持IPV4
	*
	*/
	BOOL CheckURL(const CString &szUrl, CString &szMsg);

	void OnSize(CWnd* pParent);
	void OnSize(CRect& rect);
	void OnSpecifySize(CWnd* pParent);
	bool IsCreate();
	void Clear();
	void SetEditFocus(bool focus);
	
	/**
	 * 显示指定颜色的边框
	 *
	 * @param bShow -[in] -是否显示边框
	 *
	 * @param nColor -[in] -边框颜色
	 *
	 * @return -[NONE]
	 */
	void ShowBorderLine(BOOL bShow,COLORREF nColor = RGB(53, 104, 187));
	
	/**
	 * 设置左侧偏移
	 *
	 * @param nOffset -[in] -偏移量
	 *
	 * @return -[NONE]
	 */
	void SetLeftOffset(int nOffset);
	void SetFontColor(COLORREF color);
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnLink(NMHDR *in_pNotifyHeader, LRESULT *out_pResult);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	DECLARE_MESSAGE_MAP()

private:
	CWnd*		m_pParentWnd;
	bool		m_bBottom;
	BOOL		m_ShowBorder; 
	COLORREF	m_BorderColor; 
	bool		m_bCreate;
	bool		m_bFocus;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

