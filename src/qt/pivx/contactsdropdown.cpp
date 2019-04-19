#include "qt/pivx/contactsdropdown.h"

#include <QPainter>
#include <QSizePolicy>
#include "qt/pivx/addresslabelrow.h"
#include "qt/pivx/contactdropdownrow.h"
#include "qt/pivx/qtutils.h"
#include "qt/pivx/furlistrow.h"
#include "walletmodel.h"
#include "addresstablemodel.h"

#define DECORATION_SIZE 70
#define NUM_ITEMS 3

class ContViewHolder : public FurListRow<QWidget*>
{
public:
    ContViewHolder();

    explicit ContViewHolder(bool _isLightTheme) : FurListRow(), isLightTheme(_isLightTheme){}

    ContactDropdownRow* createHolder(int pos) override{
        return new ContactDropdownRow(true, false);
    }

    void init(QWidget* holder,const QModelIndex &index, bool isHovered, bool isSelected) const override{
        ContactDropdownRow* row = static_cast<ContactDropdownRow*>(holder);
        row->update(isLightTheme, isHovered, isSelected);
        QString address = index.data(Qt::DisplayRole).toString();
        QModelIndex sibling = index.sibling(index.row(), AddressTableModel::Label);
        QString label = sibling.data(Qt::DisplayRole).toString();
        row->setData(address, label);
    }

    QColor rectColor(bool isHovered, bool isSelected) override{
        return getRowColor(isLightTheme, isHovered, isSelected);
    }

    ~ContViewHolder() override{}

    bool isLightTheme;
};


#include "qt/pivx/moc_contactsdropdown.cpp"

ContactsDropdown::ContactsDropdown(int minWidth, int minHeight, QWidget *parent) :
    QWidget(parent)
{

    this->setStyleSheet(parent->styleSheet());

    delegate = new FurAbstractListItemDelegate(
                DECORATION_SIZE,
                new ContViewHolder(isLightTheme()),
                this
    );

    setMinimumWidth(minWidth);
    setMinimumHeight(minHeight);
    setContentsMargins(0,0,0,0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    frameList = new QFrame(this);
    frameList->setProperty("cssClass", "container-border-light");
    frameList->setContentsMargins(10,10,10,10);
    frameList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    list = new QListView(frameList);
    list->setMinimumWidth(minWidth);
    list->setProperty("cssClass", "container-border-light");
    list->setItemDelegate(delegate);
    list->setIconSize(QSize(DECORATION_SIZE, DECORATION_SIZE));
    list->setMinimumHeight(NUM_ITEMS * (DECORATION_SIZE + 2));
    list->setAttribute(Qt::WA_MacShowFocusRect, false);
    list->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(list, SIGNAL(clicked(QModelIndex)), this, SLOT(handleClick(QModelIndex)));
}

void ContactsDropdown::setWalletModel(WalletModel* _model){
    model = _model->getAddressTableModel();
    this->filter = new AddressFilterProxyModel(AddressTableModel::Send, this);
    this->filter->setSourceModel(model);
    list->setModel(this->filter);
    list->setModelColumn(AddressTableModel::Address);
}

void ContactsDropdown::resizeList(int minWidth, int mintHeight){
    list->setMinimumWidth(minWidth);
    setMinimumWidth(minWidth);
    frameList->setMinimumWidth(minWidth);
    list->setMinimumHeight(mintHeight);
    list->resize(mintHeight,mintHeight);
    update();
}

void ContactsDropdown::handleClick(const QModelIndex &index){
    QModelIndex rIndex = (filter) ? filter->mapToSource(index) : index;
    QString address = rIndex.data(Qt::DisplayRole).toString();
    QModelIndex sibling = rIndex.sibling(rIndex.row(), AddressTableModel::Label);
    QString label = sibling.data(Qt::DisplayRole).toString();
    emit contactSelected(address, label);
    close();
}