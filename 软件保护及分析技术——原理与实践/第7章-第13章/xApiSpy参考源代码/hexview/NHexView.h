
#ifndef QHEXVIEW_20060506_H_
#define QHEXVIEW_20060506_H_

#include <QAbstractScrollArea>
#include <QByteArray>
#include <QString>
#include <QSharedPointer>
#include "NByteStream.h"
#include "NCommentServerInterface.h"
class QMenu;
class QTextStream;

class NHexView : public QAbstractScrollArea {
	Q_OBJECT
	
public:
    typedef qintptr     address_t;
	
public:
    NHexView(QWidget *parent = 0);
    virtual ~NHexView();
	
public:
    void setCommentServer(const QSharedPointer<NCommentServerInterface> &p);
    QSharedPointer<NCommentServerInterface> commentServer() const;

protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void resizeEvent(QResizeEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
	virtual void contextMenuEvent(QContextMenuEvent *event);

public Q_SLOTS:
	void setShowAddress(bool);
	void setShowAsciiDump(bool);
	void setShowHexDump(bool);
	void setShowComments(bool);
	//void setLineColor(QColor);
	//void setAddressColor(QColor);
	void setWordWidth(int);
	void setRowWidth(int);
	void setFont(const QFont &font);
	void repaint();
		
public:
	address_t firstVisibleAddress() const;
	address_t addressOffset() const;
	bool showHexDump() const;
	bool showAddress() const;
	bool showAsciiDump() const;
	bool showComments() const;
	QColor lineColor() const;
	QColor addressColor() const;
	int wordWidth() const;
	int rowWidth() const;
	
private:
	int row_width_;			// amount of 'words' per row
	int word_width_;		// size of a 'word' in bytes
	QColor address_color_;	// colour of the address in display
	bool show_hex_;			// should we show the hex display?
	bool show_ascii_;		// should we show the ascii display?
	bool show_address_;		// should we show the address display?
	bool show_comments_;

public:
    QSharedPointer<NByteStream> data() const { return data_; }

    void setData(const QSharedPointer<NByteStream> &d);
	void setAddressOffset(address_t offset);
	void scrollTo(unsigned int offset);
	
	address_t selectedBytesAddress() const;
	unsigned int selectedBytesSize() const;
	QByteArray selectedBytes() const;
	QByteArray allBytes() const;
	QMenu *createStandardContextMenu();

public Q_SLOTS:
	void clear();
	void selectAll();
	void deselect();
	bool hasSelectedText() const;
	void mnuSetFont();
	void mnuCopy();

private:
	void updateScrollbars();
	
	bool isSelected(int index) const;
	bool isInViewableArea(int index) const;
	
	int pixelToWord(int x, int y) const;
	
	unsigned int charsPerWord() const;
	int hexDumpLeft() const;
	int asciiDumpLeft() const;
	int commentLeft() const;
	unsigned int addressLen() const;
	int line1() const;
	int line2() const;
	int line3() const;

	unsigned int bytesPerRow() const;
	
	int dataSize() const;
	
	void drawAsciiDump(QPainter &painter, unsigned int offset, unsigned int row, int size) const;
	void drawHexDump(QPainter &painter, unsigned int offset, unsigned int row, int size, int &word_count) const;
	void drawComments(QPainter &painter, unsigned int offset, unsigned int row, int size) const;
	
	void drawAsciiDumpToBuffer(QTextStream &stream, unsigned int offset, int size) const;
	void drawHexDumpToBuffer(QTextStream &stream, unsigned int offset, int size) const;
	void drawCommentsToBuffer(QTextStream &stream, unsigned int offset, int size) const;
	
	QString formatAddress(address_t address);
    QString format_bytes(const NByteStream &data_ref, int index) const;
	
private:
	
	static bool isPrintable(unsigned char ch);
	static QAction *addToggleActionToMenu(QMenu *menu, const QString &caption, bool checked, QObject *reciever, const char *slot);
	
private:
	address_t origin_;
	address_t address_offset_;	// this is the offset that our base address is relative to
	int selection_start_;		// index of first selected word (or -1)
	int selection_end_;			// index of last selected word (or -1)
	int font_width_;			// width of a character in this font
	int font_height_;			// height of a character in this font
    QSharedPointer<NByteStream> data_;	// the current data
	
	enum {
		Highlighting_None,
		Highlighting_Data,
		Highlighting_Ascii
	} highlighting_;
	
	QColor even_word_;
	QColor non_printable_text_;
	char unprintable_char_;

	bool show_line1_;
	bool show_line2_;
	bool show_line3_;
	bool show_address_separator_;	// should we show ':' character in address to seperate high/low portions
	char address_format_string_[32];
	
    QSharedPointer<NCommentServerInterface>	comment_server_;
};

#endif
