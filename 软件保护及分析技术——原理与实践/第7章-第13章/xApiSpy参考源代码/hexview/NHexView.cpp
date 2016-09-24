#include "NHexView.h"

#include <QApplication>
#include <QClipboard>
#include <QFontDialog>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QPalette>
#include <QPixmap>
#include <QScrollBar>
#include <QSignalMapper>
#include <QTextStream>
#include <QtGlobal>

#include <cctype>
#include <climits>
#include <memory>

//------------------------------------------------------------------------------
// Name: QHexView(QWidget *parent)
// Desc: constructor
//------------------------------------------------------------------------------
NHexView::NHexView(QWidget *parent) : QAbstractScrollArea(parent),
		row_width_(16), word_width_(1), address_color_(Qt::red), 
		show_hex_(true), show_ascii_(true), show_address_(true), 
		show_comments_(true), origin_(0), address_offset_(0), 
		selection_start_(-1), selection_end_(-1), 
		highlighting_(Highlighting_None), even_word_(Qt::blue), 
		non_printable_text_(Qt::red), unprintable_char_('.'), show_line1_(true), 
		show_line2_(true), show_line3_(true) {
	
	// default to a simple monospace font
    setFont(QFont("Fixedsys", 8));
}

//------------------------------------------------------------------------------
// Name: 
// Desc: 
//------------------------------------------------------------------------------
NHexView::~NHexView() {
}


//------------------------------------------------------------------------------
// Name: 
// Desc: 
//------------------------------------------------------------------------------
QString NHexView::formatAddress(address_t address) {
    return QString("%1").arg(address,8,16,QChar('0'));
}

//------------------------------------------------------------------------------
// Name: 
// Desc: 
//------------------------------------------------------------------------------
void NHexView::repaint() {
	viewport()->repaint();
}

//------------------------------------------------------------------------------
// Name: dataSize() const
// Desc: returns how much data we are viewing
//------------------------------------------------------------------------------
int NHexView::dataSize() const {
	return data_ ? data_->size() : 0;
}

//------------------------------------------------------------------------------
// Name: setFont(const QFont &f)
// Desc: overloaded version of setFont, calculates font metrics for later
//------------------------------------------------------------------------------
void NHexView::setFont(const QFont &f) {

	// recalculate all of our metrics/offsets
	const QFontMetrics fm(f);
	font_width_		= fm.width('X');
	font_height_	= fm.height();
	
	updateScrollbars();
	
	// TODO: assert that we are using a fixed font & find out if we care?
	QAbstractScrollArea::setFont(f);
}

//------------------------------------------------------------------------------
// Name: addToggleActionToMenu(QMenu *menu, const QString &caption, bool checked, QObject *reciever, const char *slot)
// Desc: convinience function used to add a checkable menu item to the context menu
//------------------------------------------------------------------------------
QAction *NHexView::addToggleActionToMenu(QMenu *menu, const QString &caption, bool checked, QObject *reciever, const char *slot) {
	QAction *const action = new QAction(caption, menu);
    action->setCheckable(true);
    action->setChecked(checked);
	menu->addAction(action);
	connect(action, SIGNAL(toggled(bool)), reciever, slot);
	return action;
}

//------------------------------------------------------------------------------
// Name: createStandardContextMenu()
// Desc: creates the 'standard' context menu for the widget
//------------------------------------------------------------------------------
QMenu *NHexView::createStandardContextMenu() {

	QMenu *const menu = new QMenu(this);
	
	menu->addAction(tr("Set &Font"), this, SLOT(mnuSetFont()));	
	menu->addSeparator();
	addToggleActionToMenu(menu, tr("Show A&ddress"), show_address_, this, SLOT(setShowAddress(bool)));
	addToggleActionToMenu(menu, tr("Show &Hex"), show_hex_, this, SLOT(setShowHexDump(bool)));
	addToggleActionToMenu(menu, tr("Show &Ascii"), show_ascii_, this, SLOT(setShowAsciiDump(bool)));
	addToggleActionToMenu(menu, tr("Show &Comments"), show_comments_, this, SLOT(setShowComments(bool)));

	QSignalMapper *wordWidthMapper = new QSignalMapper(menu);

	QMenu *const wordMenu = new QMenu(tr("Set Word Width"), menu);
	QAction *const a1 = addToggleActionToMenu(wordMenu, tr("1 Byte"), word_width_ == 1, wordWidthMapper, SLOT(map()));
	QAction *const a2 = addToggleActionToMenu(wordMenu, tr("2 Bytes"), word_width_ == 2, wordWidthMapper, SLOT(map()));
	QAction *const a3 = addToggleActionToMenu(wordMenu, tr("4 Bytes"), word_width_ == 4, wordWidthMapper, SLOT(map()));
	QAction *const a4 = addToggleActionToMenu(wordMenu, tr("8 Bytes"), word_width_ == 8, wordWidthMapper, SLOT(map()));
	
	wordWidthMapper->setMapping(a1, 1);
	wordWidthMapper->setMapping(a2, 2);
	wordWidthMapper->setMapping(a3, 4);
	wordWidthMapper->setMapping(a4, 8);
	
	connect(wordWidthMapper, SIGNAL(mapped(int)), SLOT(setWordWidth(int)));
	
	QSignalMapper *rowWidthMapper = new QSignalMapper(menu);

	QMenu *const rowMenu = new QMenu(tr("Set Row Width"), menu);
	QAction *const a5 = addToggleActionToMenu(rowMenu, tr("1 Word"), row_width_ == 1, rowWidthMapper, SLOT(map()));
	QAction *const a6 = addToggleActionToMenu(rowMenu, tr("2 Words"), row_width_ == 2, rowWidthMapper, SLOT(map()));
	QAction *const a7 = addToggleActionToMenu(rowMenu, tr("4 Words"), row_width_ == 4, rowWidthMapper, SLOT(map()));
	QAction *const a8 = addToggleActionToMenu(rowMenu, tr("8 Words"), row_width_ == 8, rowWidthMapper, SLOT(map()));
	QAction *const a9 = addToggleActionToMenu(rowMenu, tr("16 Words"), row_width_ == 16, rowWidthMapper, SLOT(map()));

	rowWidthMapper->setMapping(a5, 1);
	rowWidthMapper->setMapping(a6, 2);
	rowWidthMapper->setMapping(a7, 4);
	rowWidthMapper->setMapping(a8, 8);
	rowWidthMapper->setMapping(a9, 16);
	
	connect(rowWidthMapper, SIGNAL(mapped(int)), SLOT(setRowWidth(int)));

	menu->addSeparator();
	menu->addMenu(wordMenu);
	menu->addMenu(rowMenu);
	
	menu->addSeparator();
	menu->addAction(tr("&Copy Selection To Clipboard"), this, SLOT(mnuCopy()));	
	
	return menu;
}

//------------------------------------------------------------------------------
// Name: contextMenuEvent(QContextMenuEvent *event)
// Desc: default context menu event, simply shows standard menu
//------------------------------------------------------------------------------
void NHexView::contextMenuEvent(QContextMenuEvent *event) {
	QMenu *const menu = createStandardContextMenu();
	menu->exec(event->globalPos());
	delete menu;
}

//------------------------------------------------------------------------------
// Name: 
// Desc: 
//------------------------------------------------------------------------------
void NHexView::mnuCopy() {
	if(hasSelectedText()) {
	
		QString s;
		QTextStream ss(&s);

		// current actual offset (in bytes)
		
		const int chars_per_row = bytesPerRow();
		unsigned int offset = verticalScrollBar()->value() * chars_per_row;

		if(origin_ != 0) {
			if(offset > 0) {	
				offset += origin_;	
				offset -= chars_per_row;
			}
		}
		
		const unsigned int end		= qMax(selection_start_, selection_end_);
		const unsigned int start	= qMin(selection_start_, selection_end_);
		const int data_size			= dataSize();
		
		// offset now refers to the first visible byte
		while(offset < end) {
		
			if((offset + chars_per_row) > start) {
		
				if(show_address_) {
					const address_t addressRVA = address_offset_ + offset;
					const QString addressBuffer = formatAddress(addressRVA);
					ss << addressBuffer << '|';
				}

				if(show_hex_) {
					drawHexDumpToBuffer(ss, offset, data_size);
					ss << "|";
				}

				if(show_ascii_) {
					drawAsciiDumpToBuffer(ss, offset, data_size);
					ss << "|";
				}

				if(show_comments_) {
					drawCommentsToBuffer(ss, offset, data_size);
				}

				ss << "\n";
			}
			offset += chars_per_row;
		}
		
		QApplication::clipboard()->setText(s);
		QApplication::clipboard()->setText(s, QClipboard::Selection);
	}
}

//------------------------------------------------------------------------------
// Name: mnuSetFont()
// Desc: slot used to set the font of the widget based on dialog selector
//------------------------------------------------------------------------------
void NHexView::mnuSetFont() {
    setFont(QFontDialog::getFont(0, font(), this));
}

//------------------------------------------------------------------------------
// Name: clear()
// Desc: clears all data from the view
//------------------------------------------------------------------------------
void NHexView::clear() {
	if(data_ != 0) {
		data_->clear();
	}

	repaint();
}

//------------------------------------------------------------------------------
// Name: hasSelectedText() const
// Desc: returns true if any text is selected
//------------------------------------------------------------------------------
bool NHexView::hasSelectedText() const {
	return !(selection_start_ == -1 || selection_end_ == -1);
}

//------------------------------------------------------------------------------
// Name: isInViewableArea(int index) const
// Desc: returns true if the word at the given index is in the viewable area
//------------------------------------------------------------------------------
bool NHexView::isInViewableArea(int index) const {

	const int firstViewableWord	= verticalScrollBar()->value() * row_width_;
	const int viewableLines		= viewport()->height() / font_height_;
	const int viewableWords		= viewableLines * row_width_;
	const int lastViewableWord	= firstViewableWord + viewableWords;
	
	return index >= firstViewableWord && index < lastViewableWord;
}

//------------------------------------------------------------------------------
// Name: keyPressEvent(QKeyEvent *event)
// Desc: 
//------------------------------------------------------------------------------
void NHexView::keyPressEvent(QKeyEvent *event) {
	if(event->modifiers() & Qt::ControlModifier) {
		switch(event->key()) {
		case Qt::Key_A:
			selectAll();
			repaint();
			break;
		case Qt::Key_Home:
			scrollTo(0);
			break;
		case Qt::Key_End:			
			scrollTo(dataSize() - bytesPerRow());
			break;
		case Qt::Key_Down:
		
			do {
				int offset = verticalScrollBar()->value() * bytesPerRow();

				if(origin_ != 0) {
					if(offset > 0) {	
						offset += origin_;	
						offset -= bytesPerRow();
					}
				}
							
				if(offset + 1 < dataSize()) {
					scrollTo(offset + 1);
				}
			} while(0);
		
			// return so we don't pass on the key event
			return;
		case Qt::Key_Up:
			do {
				int offset = verticalScrollBar()->value() * bytesPerRow();

				if(origin_ != 0) {
					if(offset > 0) {	
						offset += origin_;	
						offset -= bytesPerRow();
					}
				}
							
				if(offset > 0) {
					scrollTo(offset - 1);
				}
			} while(0);
			
			// return so we don't pass on the key event
			return;
		}
	}
		
	QAbstractScrollArea::keyPressEvent(event);
}

//------------------------------------------------------------------------------
// Name: isPrintable(unsigned char ch)
// Desc: determines if a character has a printable ascii symbol
//------------------------------------------------------------------------------
bool NHexView::isPrintable(unsigned char ch) {

	// if it's standard ascii use isprint/isspace, otherwise go with our observations
	if(ch < 0x80) {
		return std::isprint(ch) || std::isspace(ch);
	} else {
		return (ch & 0xff) >= 0xa0;
	}
}

//------------------------------------------------------------------------------
// Name: line3() const
// Desc: returns the x coordinate of the 3rd line
//------------------------------------------------------------------------------
int NHexView::line3() const {
	if(show_ascii_) {
		const int elements = bytesPerRow();
		return asciiDumpLeft() + (elements * font_width_) + (font_width_ / 2);
	} else {
		return line2();
	}
}

//------------------------------------------------------------------------------
// Name: line2() const
// Desc: returns the x coordinate of the 2nd line
//------------------------------------------------------------------------------
int NHexView::line2() const {
	if(show_hex_) {
		const int elements = row_width_ * (charsPerWord() + 1) - 1;
		return hexDumpLeft() + (elements * font_width_) + (font_width_ / 2);
	} else {
		return line1();
	}
}

//------------------------------------------------------------------------------
// Name: line1() const
// Desc: returns the x coordinate of the 1st line
//------------------------------------------------------------------------------
int NHexView::line1() const {
	if(show_address_) {
		const int elements = addressLen();
		return (elements * font_width_) + (font_width_ / 2);
	} else {
		return 0;
	}
}

//------------------------------------------------------------------------------
// Name: hexDumpLeft() const
// Desc: returns the x coordinate of the hex-dump field left edge
//------------------------------------------------------------------------------
int NHexView::hexDumpLeft() const {
	return line1() + (font_width_ / 2);
}

//------------------------------------------------------------------------------
// Name: asciiDumpLeft() const
// Desc: returns the x coordinate of the ascii-dump field left edge
//------------------------------------------------------------------------------
int NHexView::asciiDumpLeft() const {
	return line2() + (font_width_ / 2);
}

//------------------------------------------------------------------------------
// Name: commentLeft() const
// Desc: returns the x coordinate of the comment field left edge
//------------------------------------------------------------------------------
int NHexView::commentLeft() const {
	return line3() + (font_width_ / 2);
}

//------------------------------------------------------------------------------
// Name: charsPerWord() const
// Desc: returns how many characters each word takes up
//------------------------------------------------------------------------------
unsigned int NHexView::charsPerWord() const {
	return word_width_ * 2;
}

//------------------------------------------------------------------------------
// Name: addressLen() const
// Desc: returns the lenth in characters the address will take up
//------------------------------------------------------------------------------
unsigned int NHexView::addressLen() const {
	static const unsigned int addressLength = (sizeof(address_t) * CHAR_BIT) / 4;
	return addressLength + (show_address_separator_ ? 1 : 0);
}

//------------------------------------------------------------------------------
// Name: updateScrollbars()
// Desc: recalculates scrollbar maximum value base on lines total and lines viewable
//------------------------------------------------------------------------------
void NHexView::updateScrollbars() {
	const unsigned int totalLines		= dataSize() / bytesPerRow();
	const unsigned int viewableLines	= viewport()->height() / font_height_;

	unsigned int scrollMax = (totalLines > viewableLines) ? totalLines - 1 : 0;
	
	if(origin_ != 0) {
		++scrollMax;
	}
	
	verticalScrollBar()->setMaximum(scrollMax);
}

//------------------------------------------------------------------------------
// Name: scrollTo(unsigned int offset)
// Desc: scrolls view to given byte offset
//------------------------------------------------------------------------------
void NHexView::scrollTo(unsigned int offset) {

	const int bpr = bytesPerRow();
	origin_ = offset % bpr;
	address_t address = offset / bpr;

	updateScrollbars();
	
	if(origin_ != 0) {
		++address;
	}

	verticalScrollBar()->setValue(address);
	repaint();
}

//------------------------------------------------------------------------------
// Name: setShowAddress(bool show)
// Desc: sets if we are to display the address column
//------------------------------------------------------------------------------
void NHexView::setShowAddress(bool show) {
	show_address_ = show;
	repaint();
}

//------------------------------------------------------------------------------
// Name: setShowHexDump(bool show)
// Desc: sets if we are to display the hex-dump column
//------------------------------------------------------------------------------
void NHexView::setShowHexDump(bool show) {
	show_hex_ = show;
	repaint();
}

//------------------------------------------------------------------------------
// Name: setShowComments(bool show)
// Desc: sets if we are to display the comments column
//------------------------------------------------------------------------------
void NHexView::setShowComments(bool show) {
	show_comments_ = show;
	repaint();
}

//------------------------------------------------------------------------------
// Name: setShowAsciiDump(bool show)
// Desc: sets if we are to display the ascii-dump column
//------------------------------------------------------------------------------
void NHexView::setShowAsciiDump(bool show) {
	show_ascii_ = show;
	repaint();
}

//------------------------------------------------------------------------------
// Name: setRowWidth(int rowWidth)
// Desc: sets the row width (units is words)
//------------------------------------------------------------------------------
void NHexView::setRowWidth(int rowWidth) {
	row_width_ = rowWidth;
	
	updateScrollbars();
	repaint();
}

//------------------------------------------------------------------------------
// Name: setWordWidth(int wordWidth)
// Desc: sets how many bytes represent a word
//------------------------------------------------------------------------------
void NHexView::setWordWidth(int wordWidth) {
	word_width_ = wordWidth;
	
	updateScrollbars();
	repaint();
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
unsigned int NHexView::bytesPerRow() const {
	return row_width_ * word_width_;
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
int NHexView::pixelToWord(int x, int y) const {
	int word = -1;
	
	switch(highlighting_) {
	case Highlighting_Data:
		// the right edge of a box is kinda quirky, so we pretend there is one
		// extra character there
		x = qBound(line1(), x, line2() + font_width_);
		
		// the selection is in the data view portion
		x -= line1();

		// scale x/y down to character from pixels
		x /= font_width_;
		y /= font_height_;

		// make x relative to rendering mode of the bytes
		x /= (charsPerWord() + 1);
		break;
	case Highlighting_Ascii:
		x = qBound(asciiDumpLeft(), x, line3());
		
		// the selection is in the ascii view portion
		x -= asciiDumpLeft();

		// scale x/y down to character from pixels
		x /= font_width_;
		y /= font_height_;

		// make x relative to rendering mode of the bytes
		x /= word_width_;
		break;
	default:
		Q_ASSERT(0);
		break;
	}

	// starting offset in bytes
	unsigned int startOffset = verticalScrollBar()->value() * bytesPerRow();
		
	// take into account the origin
	if(origin_ != 0) {
		if(startOffset > 0) {	
			startOffset += origin_;	
			startOffset -= bytesPerRow();
		}
	}
	
	// convert byte offset to word offset, rounding up
	startOffset /= word_width_;
	
	if((origin_ % word_width_) != 0) {
		startOffset += 1;
	}

	word = ((y * row_width_) + x) + startOffset;
	
	
	
	return word;
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
void NHexView::mouseDoubleClickEvent(QMouseEvent *event) {
	if(event->button() == Qt::LeftButton) {
		const int x = event->x();
		const int y = event->y();
		if(x >= line1() && x < line2()) {

			highlighting_ = Highlighting_Data;

			const int offset = pixelToWord(x, y);
			int byteOffset = offset * word_width_;
			if(origin_) {
				if(origin_ % word_width_) {
					byteOffset -= word_width_ - (origin_ % word_width_);
				}
			}
			
			selection_start_ = byteOffset;
			selection_end_ = selection_start_ + word_width_;
			repaint();
		}
	}
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
void NHexView::mousePressEvent(QMouseEvent *event) {
	if(event->button() == Qt::LeftButton) {
		const int x = event->x();
		const int y = event->y();

		if(x < line2()) {
			highlighting_ = Highlighting_Data;
		} else if(x >= line2()) {
			highlighting_ = Highlighting_Ascii;
		}

		const int offset = pixelToWord(x, y);
		int byteOffset = offset * word_width_;
		if(origin_) {
			if(origin_ % word_width_) {
				byteOffset -= word_width_ - (origin_ % word_width_);
			}
		}

		if(offset < dataSize()) {
			selection_start_ = selection_end_ = byteOffset;
		} else {
			selection_start_ = selection_end_ = -1;
		}
		repaint();
	}
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
void NHexView::mouseMoveEvent(QMouseEvent *event) {
	if(highlighting_ != Highlighting_None) {
		const int x = event->x();
		const int y = event->y();

		const int offset = pixelToWord(x, y);

		if(selection_start_ != -1) {
			if(offset == -1) {
			
			
				selection_end_ = (row_width_ - selection_start_) + selection_start_;
			} else {
			
				int byteOffset = (offset * word_width_);
				
				if(origin_) {
					if(origin_ % word_width_) {
						byteOffset -= word_width_ - (origin_ % word_width_);
					}
					
				}
				selection_end_ = byteOffset;
			}
			
			
			if(selection_end_ < 0) {
				selection_end_ = 0;
			}
			
			if(!isInViewableArea(selection_end_)) {
				// TODO: scroll to an appropriate location
			}
			
		}
		repaint();
	}
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
void NHexView::mouseReleaseEvent(QMouseEvent *event) {
	if(event->button() == Qt::LeftButton) {
		highlighting_ = Highlighting_None;
	}
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
void NHexView::setData(const QSharedPointer<NByteStream> &d) {
	data_ = d;
	
	deselect();
	updateScrollbars();
	repaint();
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
void NHexView::resizeEvent(QResizeEvent *) {
	updateScrollbars();
}

//------------------------------------------------------------------------------
// Name: setAddressOffset(address_t offset)
//------------------------------------------------------------------------------
void NHexView::setAddressOffset(address_t offset) {
	address_offset_ = offset;
}

//------------------------------------------------------------------------------
// Name: isSelected(int index) const
//------------------------------------------------------------------------------
bool NHexView::isSelected(int index) const {

	bool ret = false;
	if(index < static_cast<int>(dataSize())) {
		if(selection_start_ != selection_end_) {
			if(selection_start_ < selection_end_) {
				ret = (index >= selection_start_ && index < selection_end_);
			} else {
				ret = (index >= selection_end_ && index < selection_start_);
			}
		}
	}
	return ret;
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
void NHexView::drawComments(QPainter &painter, unsigned int offset, unsigned int row, int size) const {
	
	Q_UNUSED(size);
	
	painter.setPen(QPen(palette().text().color()));
	
	const address_t address	= address_offset_ + offset;
    const QString comment = comment_server_->comment(address, word_width_);;

	painter.drawText(
		commentLeft(),
		row, 
		comment.length() * font_width_,
		font_height_,
		Qt::AlignTop,
		comment
		);
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
void NHexView::drawAsciiDumpToBuffer(QTextStream &stream, unsigned int offset, int size) const {
    const NByteStream &data_ref(*data_);
	
	// i is the byte index
	const int chars_per_row = bytesPerRow();
	
	for(int i = 0; i < chars_per_row; ++i) {
		
		const int index = offset + i;

		if(index < size) {
			
			if(isSelected(index)) {
				const unsigned char ch = data_ref[index];
				const bool printable = isPrintable(ch) && ch != '\f' && ch != '\t' && ch != '\r' && ch != '\n' && ch < 0x80;
				const char byteBuffer(printable ? ch : unprintable_char_);
				stream << byteBuffer;
			} else {
				stream << ' ';
			}
		} else {
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
void NHexView::drawCommentsToBuffer(QTextStream &stream, unsigned int offset, int size) const {
	Q_UNUSED(size);
	const address_t address	= address_offset_ + offset;
    const QString comment = comment_server_->comment(address, word_width_);;
	stream << comment;
}

//------------------------------------------------------------------------------
// Name: format_bytes(const C &data_ref, int index) const
// Desc: formats bytes in a way that's suitable for rendering in a hexdump
//       having this as a separate function serves two purposes.
//       #1 no code duplication between the buffer and QPainter versions
//       #2 this encourages NRVO of the return value more than an integrated
//------------------------------------------------------------------------------
QString NHexView::format_bytes(const NByteStream &data_ref, int index) const {
	
	union {
		quint64 q;
		quint32 d;
		quint16 w;
		quint8	b;
	} value = { 0 };
	
	char byte_buffer[32];

	switch(word_width_) {
	case 1:
		value.b |= data_ref[index + 0];
		qsnprintf(byte_buffer, sizeof(byte_buffer), "%02x", value.b);
		break;
	case 2:
		value.w |= data_ref[index + 0];
		value.w |= data_ref[index + 1] << 8;
		qsnprintf(byte_buffer, sizeof(byte_buffer), "%04x", value.w);
		break;
	case 4:
		value.d |= data_ref[index + 0];
		value.d |= data_ref[index + 1] << 8;
		value.d |= data_ref[index + 2] << 16;
		value.d |= data_ref[index + 3] << 24;
		qsnprintf(byte_buffer, sizeof(byte_buffer), "%08x", value.d);
		break;
	case 8:
		// we need the cast to ensure that it won't assume 32-bit
		// and drop bits shifted more that 31
		value.q |= static_cast<quint64>(data_ref[index + 0]);
		value.q |= static_cast<quint64>(data_ref[index + 1]) << 8;
		value.q |= static_cast<quint64>(data_ref[index + 2]) << 16;
		value.q |= static_cast<quint64>(data_ref[index + 3]) << 24;
		value.q |= static_cast<quint64>(data_ref[index + 4]) << 32;
		value.q |= static_cast<quint64>(data_ref[index + 5]) << 40;
		value.q |= static_cast<quint64>(data_ref[index + 6]) << 48;
		value.q |= static_cast<quint64>(data_ref[index + 7]) << 56;
		qsnprintf(byte_buffer, sizeof(byte_buffer), "%016llx", value.q);
		break;
	}

	return byte_buffer;
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
void NHexView::drawHexDumpToBuffer(QTextStream &stream, unsigned int offset, int size) const {
	
	Q_UNUSED(size);

    const NByteStream &data_ref(*data_);

	// i is the word we are currently rendering
	for(int i = 0; i < row_width_; ++i) {
		
		// index of first byte of current 'word'
		const int index = offset + (i * word_width_);

		// equal <=, not < because we want to test the END of the word we
		// about to render, not the start, it's allowed to end at the very last 
		// byte
		if(index + word_width_ <= size) {
			const QString byteBuffer = format_bytes(data_ref, index);

			if(isSelected(index)) {
				stream << byteBuffer;
			} else {
				stream << QString(byteBuffer.length(), ' ');
			}
			
			if(i != (row_width_ - 1)) {
				stream << ' ';
			}
		} else {
			break;	
		}
	}
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
void NHexView::drawHexDump(QPainter &painter, unsigned int offset, unsigned int row, int size, int &word_count) const {

    const NByteStream &data_ref(*data_);
	const int hex_dump_left = hexDumpLeft();

	// i is the word we are currently rendering
	for(int i = 0; i < row_width_; ++i) {
		
		// index of first byte of current 'word'
		const int index = offset + (i * word_width_);
		
		// equal <=, not < because we want to test the END of the word we
		// about to render, not the start, it's allowed to end at the very last 
		// byte
		if(index + word_width_ <= size) {
			const QString byteBuffer = format_bytes(data_ref, index);

			const int drawLeft = hex_dump_left + (i * (charsPerWord() + 1) * font_width_);

			if(isSelected(index)) {
				painter.fillRect(
					drawLeft,
					row,
					charsPerWord() * font_width_,
					font_height_,
					palette().highlight()
				);

				// should be highlight the space between us and the next word?
				if(i != (row_width_ - 1)) {
					if(isSelected(index + 1)) {
						painter.fillRect(
							drawLeft + font_width_,
							row,
							charsPerWord() * font_width_,
							font_height_,
							palette().highlight()
							);
					}
				}
				
				painter.setPen(QPen(palette().highlightedText().color()));
			} else {
				painter.setPen(QPen((word_count & 1) ? even_word_ : palette().text().color()));
			}

			painter.drawText(
				drawLeft,
				row, 
				byteBuffer.length() * font_width_,
				font_height_,
				Qt::AlignTop,
				byteBuffer
				);

			++word_count;
		} else {
			break;	
		}
	}
}

//------------------------------------------------------------------------------
// Name: drawAsciiDump(QPainter &painter, unsigned int offset, unsigned int row, int size) const
//------------------------------------------------------------------------------
void NHexView::drawAsciiDump(QPainter &painter, unsigned int offset, unsigned int row, int size) const {
	
    const NByteStream &data_ref(*data_);
	const int ascii_dump_left = asciiDumpLeft();
	
	// i is the byte index
	const int chars_per_row = bytesPerRow();
	
	for(int i = 0; i < chars_per_row; ++i) {
		
		const int index = offset + i;

		if(index < size) {
			
			const char ch        = data_ref[index];
			const int drawLeft   = ascii_dump_left + i * font_width_;
			const bool printable = isPrintable(ch);

			// drawing a selected character
			if(isSelected(index)) {

				painter.fillRect(
					drawLeft,
					row,
					font_width_,
					font_height_,
					palette().highlight()
					);

				painter.setPen(QPen(palette().highlightedText().color()));

			} else {
				painter.setPen(QPen(printable ? palette().text().color() : non_printable_text_));
			}
			
			const QString byteBuffer(printable ? ch : unprintable_char_);

			painter.drawText(
				drawLeft,
				row,
				font_width_,
				font_height_,
				Qt::AlignTop, 
				byteBuffer
				);
		} else {
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Name: paintEvent(QPaintEvent *)
//------------------------------------------------------------------------------
void NHexView::paintEvent(QPaintEvent *) {

	QPainter painter(viewport());

	int word_count = 0;

	// pixel offset of this row
	unsigned int row = 0;
	
	const int chars_per_row = bytesPerRow();
		
	// current actual offset (in bytes)
	unsigned int offset = verticalScrollBar()->value() * chars_per_row;
	
	if(origin_ != 0) {
		if(offset > 0) {	
			offset += origin_;	
			offset -= chars_per_row;
		} else {
			origin_ = 0;
			updateScrollbars();
		}
	}
	
	const unsigned int data_size		= static_cast<unsigned int>(dataSize());
	const unsigned int widget_height	= static_cast<unsigned int>(height());
	
	while(row + font_height_ < widget_height && offset < data_size) {
	
		if(show_address_) {
			const address_t addressRVA = address_offset_ + offset;
			const QString addressBuffer = formatAddress(addressRVA);
			painter.setPen(QPen(address_color_));
			painter.drawText(0, row, addressBuffer.length() * font_width_, font_height_, Qt::AlignTop, addressBuffer);
		}
				
		if(show_hex_) {
			drawHexDump(painter, offset, row, data_size, word_count);
		}

		if(show_ascii_) {
			drawAsciiDump(painter, offset, row, data_size);
		}
		
		if(show_comments_ && comment_server_) {
			drawComments(painter, offset, row, data_size);
		}

		offset += chars_per_row;
		row += font_height_;
	}
		
	painter.setPen(QPen(palette().shadow().color()));
	
	if(show_address_ && show_line1_) {
		const int line1_x = line1();
		painter.drawLine(line1_x, 0, line1_x, widget_height);
	}
	
	if(show_hex_ && show_line2_) {
		const int line2_x = line2();
		painter.drawLine(line2_x, 0, line2_x, widget_height);
	}
	
	if(show_ascii_ && show_line3_) {
		const int line3_x = line3();	
		painter.drawLine(line3_x, 0, line3_x, widget_height);
	}
}

//------------------------------------------------------------------------------
// Name: selectAll()
//------------------------------------------------------------------------------
void NHexView::selectAll() {
	selection_start_	= 0;
	selection_end_		= dataSize();
}

//------------------------------------------------------------------------------
// Name: deselect()
//------------------------------------------------------------------------------
void NHexView::deselect() {
	selection_start_	= -1;
	selection_end_		= -1;
}

//------------------------------------------------------------------------------
// Name: allBytes() const
//------------------------------------------------------------------------------
QByteArray NHexView::allBytes() const {
	QByteArray ret;
    const NByteStream &data_ref(*data_);
	const int size = dataSize();
	
	for(int i = 0; i < size; ++i) {
		ret.push_back(data_ref[i]);
	}
	
	return ret;
}

//------------------------------------------------------------------------------
// Name: selectedBytes() const
//------------------------------------------------------------------------------
QByteArray NHexView::selectedBytes() const {
	QByteArray ret;
	if(hasSelectedText()) {
        const NByteStream &data_ref(*data_);
		
		int s = qMin(selection_start_, selection_end_);
		int e = qMax(selection_start_, selection_end_);
	
		for(int i = s; i != e; ++i) {
			ret.push_back(data_ref[i]);
		}
	}
	
	return ret;
}

//------------------------------------------------------------------------------
// Name: selectedBytesAddress() const
//------------------------------------------------------------------------------
NHexView::address_t NHexView::selectedBytesAddress() const {
	const address_t selectBase = qMin(selection_start_, selection_end_);
	return selectBase + address_offset_;
}

//------------------------------------------------------------------------------
// Name: selectedBytesSize() const
//------------------------------------------------------------------------------
unsigned int NHexView::selectedBytesSize() const {
	
	unsigned int ret;
	if(selection_end_ > selection_start_) {
		ret = selection_end_ - selection_start_;
	} else {
		ret = selection_start_ - selection_end_;
	}
	
	return ret;
}

//------------------------------------------------------------------------------
// Name: addressOffset() const
//------------------------------------------------------------------------------
NHexView::address_t NHexView::addressOffset() const {
	return address_offset_;
}

//------------------------------------------------------------------------------
// Name: setCommentServer(const QSharedPointer<CommentServerInterface> &p)
//------------------------------------------------------------------------------
void NHexView::setCommentServer(const QSharedPointer<NCommentServerInterface> &p) {
	comment_server_ = p;
}

//------------------------------------------------------------------------------
// Name: commentServer() const
//------------------------------------------------------------------------------
QSharedPointer<NCommentServerInterface> NHexView::commentServer() const {
	return comment_server_;
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
bool NHexView::showHexDump() const {
	return show_hex_;
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
bool NHexView::showAddress() const {
	return show_address_;
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
bool NHexView::showAsciiDump() const {
	return show_ascii_;
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
bool NHexView::showComments() const {
	return show_comments_;
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
int NHexView::wordWidth() const {
	return word_width_;
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
int NHexView::rowWidth() const {
	return row_width_;
}

//------------------------------------------------------------------------------
// Name: 
//------------------------------------------------------------------------------
NHexView::address_t NHexView::firstVisibleAddress() const {
	// current actual offset (in bytes)
	const int chars_per_row = bytesPerRow();
	unsigned int offset = verticalScrollBar()->value() * chars_per_row;
	
	if(origin_ != 0) {
		if(offset > 0) {	
			offset += origin_;	
			offset -= chars_per_row;
		}
	}
	
	return offset + addressOffset();
}
