class QWT_EXPORT QwtMyNeedle: public QwtDialNeedle
{
public:

QwtMySymbol(QImage image, const QColor &light = Qt::red,
const QColor &dark = Qt::gray);

virtual void draw(QPainter *, const QPoint &, int length,
double direction, QPalette::ColorGroup = QPalette::Active) const;

static void drawMyNeedle(QPainter *,
const QPalette &, QPalette::ColorGroup,
const QPoint &, int length, double direction);
private:
QImage image;
};
