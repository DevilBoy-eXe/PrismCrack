#pragma once

#include <QListView>
#include <QLineEdit>
#include <QScrollBar>

struct CategorizedViewRoles
{
	enum
	{
		CategoryRole = Qt::UserRole,
		ProgressValueRole,
		ProgressMaximumRole
	};
};

struct Group;

class GroupView : public QAbstractItemView
{
	Q_OBJECT

public:
	GroupView(QWidget *parent = 0);
	~GroupView();

	virtual QRect geometryRect(const QModelIndex &index) const;
	virtual QRect visualRect(const QModelIndex &index) const override;
	QModelIndex indexAt(const QPoint &point) const;
	void setSelection(const QRect &rect,
					  const QItemSelectionModel::SelectionFlags commands) override;

	virtual int horizontalOffset() const override
	{
		return horizontalScrollBar()->value();
	}

	virtual int verticalOffset() const override
	{
		return verticalScrollBar()->value();
	}

	virtual void scrollContentsBy(int dx, int dy) override
	{
		scrollDirtyRegion(dx, dy);
		viewport()->scroll(dx, dy);
	}

	/*
	 * TODO!
	 */
	virtual void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible) override
	{
		return;
	}

	virtual QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
		override;

	virtual QRegion visualRegionForSelection(const QItemSelection &selection) const override;


protected
slots:
	void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
					 const QVector<int> &roles);
	virtual void rowsInserted(const QModelIndex &parent, int start, int end);
	virtual void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
	virtual void updateGeometries();

protected:
	virtual bool isIndexHidden(const QModelIndex &index) const;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;

	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void dragLeaveEvent(QDragLeaveEvent *event) override;
	void dropEvent(QDropEvent *event) override;

	void startDrag(Qt::DropActions supportedActions) override;

private:
	friend struct Group;

	QList<Group *> m_categories;

	int m_leftMargin;
	int m_rightMargin;
	int m_bottomMargin;
	int m_categoryMargin;

	// bool m_updatesDisabled;

	Group *category(const QModelIndex &index) const;
	Group *category(const QString &cat) const;
	Group *categoryAt(const QPoint &pos) const;

	int itemsPerRow() const;
	int contentWidth() const;

private:
	int itemWidth() const;
	int categoryRowHeight(const QModelIndex &index) const;

	/*QLineEdit *m_categoryEditor;
	Category *m_editedCategory;
	void startCategoryEditor(Category *category);

private slots:
	void endCategoryEditor();*/

private: /* variables */
	QPoint m_pressedPosition;
	QPersistentModelIndex m_pressedIndex;
	bool m_pressedAlreadySelected;
	Group *m_pressedCategory;
	QItemSelectionModel::SelectionFlag m_ctrlDragSelectionFlag;
	QPoint m_lastDragPosition;
	int m_spacing = 5;

private: /* methods */
	QPair<int, int> categoryInternalPosition(const QModelIndex &index) const;
	int categoryInternalRowTop(const QModelIndex &index) const;
	int itemHeightForCategoryRow(const Group *category, const int internalRow) const;

	QPixmap renderToPixmap(const QModelIndexList &indices, QRect *r) const;
	QList<QPair<QRect, QModelIndex>> draggablePaintPairs(const QModelIndexList &indices,
														 QRect *r) const;

	bool isDragEventAccepted(QDropEvent *event);

	QPair<Group *, int> rowDropPos(const QPoint &pos);

	QPoint offset() const;
};
