// SPDX-License-Identifier: GPL-3.0-only

#include "tag_editor_array_widget.hpp"
#include "tag_editor_edit_widget_view.hpp"
#include "../tag_editor_window.hpp"
#include "../../tree/tag_tree_window.hpp"
#include <invader/tag/parser/parser_struct.hpp>

#include <QSpacerItem>
#include <QFontDatabase>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QApplication>

namespace Invader::EditQt {
    TagEditorArrayWidget::TagEditorArrayWidget(QWidget *parent, Parser::ParserStructValue *value, TagEditorWindow *editor_window) : TagEditorWidget(parent, value, editor_window) {
        this->vbox_layout = new QVBoxLayout();
        this->vbox_layout->setMargin(8);
        this->vbox_layout->setSpacing(2);
        this->reflexive_index = new QComboBox();
        this->item_model = nullptr;
        this->read_only = value->is_read_only() && editor_window->get_parent_window()->safeguards();

        // Set our header stuff
        QFrame *header = new QFrame();
        QHBoxLayout *header_layout = new QHBoxLayout();
        QLabel *title_label = new QLabel();
        title_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        this->setLayout(this->vbox_layout);
        this->vbox_layout->addWidget(header);
        header->setLayout(header_layout);
        header_layout->addWidget(title_label);
        header_layout->addWidget(this->reflexive_index);
        header->setFrameStyle(QFrame::Panel | QFrame::Raised);
        header->setLineWidth(2);
        header_layout->setMargin(8);
        header_layout->setMargin(8);

        QPalette palette;
        header->setAutoFillBackground(true);
        palette.setColor(QPalette::Window, QApplication::palette().color(QPalette::Light));
        header->setPalette(palette);

        // Set size stuff
        title_label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        int title_width = title_label->fontMetrics().boundingRect("MMMM").width() * 5;
        title_label->setMinimumWidth(title_width);
        title_label->setMaximumWidth(title_width);
        this->reflexive_index->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
        this->reflexive_index->setMinimumWidth(title_width * 3 / 2);
        this->reflexive_index->setMaximumWidth(title_width * 3 / 2);
        this->reflexive_index->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        title_label->setText(value->get_name());

        // Font stuff
        QFont font = QFontDatabase::systemFont(QFontDatabase::TitleFont);
        font.setBold(true);
        font.setCapitalization(QFont::AllUppercase);
        title_label->setFont(font);

        // Buttons
        this->add_button = new QPushButton("Add New");
        header_layout->addWidget(this->add_button);
        this->add_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        this->insert_button = new QPushButton("Insert New");
        header_layout->addWidget(this->insert_button);
        this->insert_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        this->duplicate_button = new QPushButton("Duplicate");
        header_layout->addWidget(this->duplicate_button);
        this->duplicate_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        this->shift_up_button = new QPushButton("Shift Up");
        header_layout->addWidget(this->shift_up_button);
        this->shift_up_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        this->shift_down_button = new QPushButton("Shift Down");
        header_layout->addWidget(this->shift_down_button);
        this->shift_down_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        this->clear_button = new QPushButton("Clear");
        header_layout->addWidget(this->clear_button);
        this->clear_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        this->delete_button = new QPushButton("Delete");
        header_layout->addWidget(this->delete_button);
        this->delete_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        this->delete_all_button = new QPushButton("Delete All");
        header_layout->addWidget(this->delete_all_button);
        this->delete_all_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        // If we're read only, disable buttons
        if(this->read_only) {
            this->add_button->setEnabled(false);
            this->insert_button->setEnabled(false);
            this->delete_button->setEnabled(false);
            this->duplicate_button->setEnabled(false);
            this->shift_up_button->setEnabled(false);
            this->shift_down_button->setEnabled(false);
            this->clear_button->setEnabled(false);
            this->delete_all_button->setEnabled(false);
        }
        // Otherwise, connect things
        else {
            connect(this->add_button, &QPushButton::clicked, this, &TagEditorArrayWidget::perform_add);
            connect(this->insert_button, &QPushButton::clicked, this, &TagEditorArrayWidget::perform_insert);
            connect(this->delete_button, &QPushButton::clicked, this, &TagEditorArrayWidget::perform_delete);
            connect(this->duplicate_button, &QPushButton::clicked, this, &TagEditorArrayWidget::perform_duplicate);
            connect(this->shift_up_button, &QPushButton::clicked, this, &TagEditorArrayWidget::perform_shift_up);
            connect(this->shift_down_button, &QPushButton::clicked, this, &TagEditorArrayWidget::perform_shift_down);
            connect(this->clear_button, &QPushButton::clicked, this, &TagEditorArrayWidget::perform_clear);
            connect(this->delete_all_button, &QPushButton::clicked, this, &TagEditorArrayWidget::perform_delete_all);
        }

        // Set this stuff up
        this->regenerate_enum();
        this->regenerate_widget();
        connect(this->reflexive_index, &QComboBox::currentTextChanged, this, &TagEditorArrayWidget::regenerate_widget);

        header_layout->addStretch(1);
    }

    int TagEditorArrayWidget::current_index() const noexcept {
        return this->reflexive_index->currentIndex();
    }

    void TagEditorArrayWidget::perform_add() {
        int index = this->get_struct_value()->get_array_size();
        this->get_struct_value()->insert_objects_in_array(index, 1);
        this->regenerate_enum();
        this->value_changed();

        this->reflexive_index->blockSignals(true);
        this->reflexive_index->setCurrentIndex(index);
        this->reflexive_index->blockSignals(false);
        this->regenerate_widget();
    }

    void TagEditorArrayWidget::perform_insert() {
        int index = this->current_index();
        this->get_struct_value()->insert_objects_in_array(index, 1);
        this->regenerate_enum();
        this->value_changed();

        this->reflexive_index->blockSignals(true);
        this->reflexive_index->setCurrentIndex(index);
        this->reflexive_index->blockSignals(false);
        this->regenerate_widget();
    }

    void TagEditorArrayWidget::perform_delete() {
        // Delete the object at the current index
        int index = this->current_index();
        this->get_struct_value()->delete_objects_in_array(static_cast<std::size_t>(index), 1);

        // Decrement if possible
        if(--index < 0 && this->get_struct_value()->get_array_size() > 0) {
            index = 0;
        }

        this->regenerate_enum();
        this->value_changed();

        this->reflexive_index->blockSignals(true);
        this->reflexive_index->setCurrentIndex(index);
        this->reflexive_index->blockSignals(false);
        this->regenerate_widget();
    }

    void TagEditorArrayWidget::perform_duplicate() {
        auto index = static_cast<std::size_t>(this->current_index());
        this->get_struct_value()->duplicate_objects_in_array(index, index, 1);

        this->regenerate_enum();
        this->value_changed();

        this->reflexive_index->blockSignals(true);
        this->reflexive_index->setCurrentIndex(index + 1);
        this->reflexive_index->blockSignals(false);
        this->regenerate_widget();
    }

    void TagEditorArrayWidget::perform_clear() {
        auto index = static_cast<std::size_t>(this->current_index());
        this->get_struct_value()->delete_objects_in_array(index, 1);
        this->get_struct_value()->insert_objects_in_array(index, 1);

        this->regenerate_enum();
        this->value_changed();

        this->reflexive_index->blockSignals(true);
        this->reflexive_index->setCurrentIndex(index);
        this->reflexive_index->blockSignals(false);
        this->regenerate_widget();
    }

    void TagEditorArrayWidget::perform_delete_all() {
        this->get_struct_value()->delete_objects_in_array(0, this->get_struct_value()->get_array_size());
        this->regenerate_enum();
        this->value_changed();

        this->reflexive_index->blockSignals(true);
        this->reflexive_index->setCurrentIndex(-1);
        this->reflexive_index->blockSignals(false);
        this->regenerate_widget();
    }

    void TagEditorArrayWidget::perform_shift_up() {
        auto index = static_cast<std::size_t>(this->current_index());
        this->get_struct_value()->duplicate_objects_in_array(index, index + 2, 1);
        this->get_struct_value()->delete_objects_in_array(index, 1);

        this->regenerate_enum();
        this->value_changed();

        this->reflexive_index->blockSignals(true);
        this->reflexive_index->setCurrentIndex(index + 1);
        this->reflexive_index->blockSignals(false);
        this->regenerate_widget();
    }

    void TagEditorArrayWidget::perform_shift_down() {
        auto index = static_cast<std::size_t>(this->current_index());
        this->get_struct_value()->duplicate_objects_in_array(index, index - 1, 1);
        this->get_struct_value()->delete_objects_in_array(index + 1, 1);

        this->regenerate_enum();
        this->value_changed();

        this->reflexive_index->blockSignals(true);
        this->reflexive_index->setCurrentIndex(index - 1);
        this->reflexive_index->blockSignals(false);
        this->regenerate_widget();
    }

    void TagEditorArrayWidget::regenerate_widget() {
        delete this->tag_view_widget;

        this->set_buttons_enabled();

        // Make sure we got it!
        int index = this->current_index();
        std::size_t count = this->get_struct_value()->get_array_size();
        if(index < 0 || static_cast<std::size_t>(index) > count) {
            this->tag_view_widget = nullptr;
            return;
        }

        // Get it!
        auto index_unsigned = static_cast<std::size_t>(index);
        auto &s = this->get_struct_value()->get_object_in_array(index_unsigned);
        this->tag_view_widget = new TagEditorEditWidgetView(this, s.get_values(), this->get_editor_window(), false);
        this->vbox_layout->addWidget(this->tag_view_widget);
    }

    void TagEditorArrayWidget::regenerate_enum() {
        this->reflexive_index->blockSignals(true);
        this->reflexive_index->setUpdatesEnabled(false);

        // Use a QStandardItemModel - it's a bit faster than adding directly, especially on Windows for whatever reason
        delete this->item_model;
        this->item_model = new QStandardItemModel(this->reflexive_index);

        auto *struct_value = this->get_struct_value();
        std::size_t count = struct_value->get_array_size();
        bool has_title = false;
        for(std::size_t i = 0; i < count; i++) {
            // See if we have a title
            if(i == 0) {
                has_title = struct_value->get_object_in_array(i).has_title();
            }
            // If so, show that alongside the index
            if(has_title) {
                const char *title_value = struct_value->get_object_in_array(i).title();
                if(title_value == nullptr || title_value[0] == 0) {
                    this->item_model->appendRow(new QStandardItem(QString::number(i)));
                }
                else {
                    char title[256];
                    std::snprintf(title, sizeof(title), "%zu (%s)", i, title_value);
                    this->item_model->appendRow(new QStandardItem(title));
                }
            }
            // If not, well... darn
            else {
                this->item_model->appendRow(new QStandardItem(QString::number(i)));
            }
        }

        this->reflexive_index->setModel(this->item_model);
        this->reflexive_index->setEnabled(count > 0);

        this->reflexive_index->setUpdatesEnabled(true);
        this->reflexive_index->blockSignals(false);
    }

    void TagEditorArrayWidget::set_buttons_enabled() {
        auto *value = this->get_struct_value();

        // If we're read only, don't bother
        if(this->read_only) {
            return;
        }

        std::size_t count = value->get_array_size();
        int index = this->current_index();
        auto index_unsigned = static_cast<std::size_t>(index);
        bool selection = index >= 0;

        auto min = value->get_array_minimum_size();
        auto max = value->get_array_maximum_size();

        this->delete_button->setEnabled(selection && index_unsigned >= min);
        this->delete_all_button->setEnabled(count > 0 && min == 0);
        this->shift_down_button->setEnabled(selection && index_unsigned > 0);
        this->shift_up_button->setEnabled(selection && index_unsigned + 1 < count);
        this->add_button->setEnabled(count < max);
        this->insert_button->setEnabled(count < max);
        this->duplicate_button->setEnabled(index >= 0 && this->add_button->isEnabled());
        this->clear_button->setEnabled(index >= 0);
    }

    void TagEditorArrayWidget::update_text() {
        this->reflexive_index->blockSignals(true);

        // We've changed a value that changes some reflexive bullshit
        QString new_title;
        std::size_t i = this->reflexive_index->currentIndex();
        const char *title_value = this->get_struct_value()->get_object_in_array(i).title();
        if(title_value == nullptr || title_value[0] == 0) {
            new_title = QString::number(i);
        }
        else {
            char title[256];
            std::snprintf(title, sizeof(title), "%zu (%s)", i, title_value);
            new_title = title;
        }
        this->reflexive_index->setItemText(this->reflexive_index->currentIndex(), new_title);

        this->reflexive_index->blockSignals(false);
    }
}