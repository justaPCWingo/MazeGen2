#pragma once
#include<QPushButton>

//good hints: http://stackoverflow.com/questions/282938/show-result-of-color-selection-in-qt

/*!
 * \brief Custom QT implementation of a color well button.
 * \author P Wingo
 */
class ColorWellButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ColorWellButton(QWidget *parent = 0);
    QColor selectedColor() const;

signals:
    /*!
     * \brief Signal emitted when a new color is picked by user.
     */
    void colorChanged(QColor);

public slots:
    void setColor(const QColor & c);
    QColor color() const;
    void runColorPicker();
    void paintEvent(QPaintEvent* evt) Q_DECL_OVERRIDE;


private:
    QColor _color;  //!< The currently selected color.
    QString _label; //!< The label to apply to the color dialog.
};

