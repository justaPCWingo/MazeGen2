#include "ColorWellButton.h"

#include<qcolordialog.h>
#include<QPaintEvent>
#include<QStylePainter>
#include<QStyleOptionButton>

/*!
 * \brief Standard QWidget constructor.
 * \param parent Pointer to the parent widget (if any).
 */
ColorWellButton::ColorWellButton(QWidget *parent)
    :QPushButton(parent),_color(Qt::black)
{

    //wire up color picker
    connect(this,SIGNAL(clicked()),this,SLOT(runColorPicker()));
}

/*!
 * \brief Retrieve the selected / displayed color.
 * \return The color that was either set through setColor(),
 * or the color chosen by the user in the color picker dialog.
 */
QColor ColorWellButton::selectedColor() const
{
    return _color;
}

/*!
 * \brief Sets the currently selected color.
 * \param c The color to currently select.
 */
void ColorWellButton::setColor(const QColor &c)
{
    _color=c;
    this->update();
    colorChanged(_color);
}

/*!
 * \brief Retrieve user-selected color from button.
 * \return The currently selected color.
 */
QColor ColorWellButton::color() const
{
    return _color;
}

/*!
 * \brief Display the color picker dialog, and store the
 * chosen color, if any was selected.
 */
void ColorWellButton::runColorPicker()
{
    QColor newColor=QColorDialog::getColor(_color,this,this->text());
    if(newColor.isValid())
        setColor(newColor);
}

/*!
 * \brief Qt Event handler; draws button.
 * \param evt The paint-triggering event.
 */
void ColorWellButton::paintEvent(QPaintEvent *evt)
{

    QRect colorBox=evt->rect();

    const int MARGIN=6;
    colorBox.adjust(MARGIN,MARGIN,-MARGIN-1,-MARGIN-1);

    QStylePainter painter(this);

    QStyleOptionButton option;
    initStyleOption(&option);
    option.text=QString();

    painter.drawControl(QStyle::CE_PushButton, option);


    if(this->isEnabled())
    {
        painter.setPen(Qt::black);
        painter.setBrush(_color);
    }
    else
    {
        painter.setPen(Qt::darkGray);
        painter.setBrush(Qt::transparent);
    }
    painter.drawRect(colorBox);
}
