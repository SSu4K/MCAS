#include <QFile>
#include <QTextStream>

#include "instructionsetmodel.h"
#include "Assembly/instructiondefinition.h"

const static QString INSTRUCTION_SET_HEADER = "[Instruction Set]";
const static QChar HEADER_PREFIX = '[';
const static QChar COMMENT_PREFIX = ';';
const static QChar DELIMITER = '|';

namespace Models {

using namespace Assembly;

InstructionSetModel::InstructionSetModel(InstructionSet* instructionSet,
                                         QObject* parent)
    : TextTableModel(parent),
    instructionSet(instructionSet)
{
    sectionHeader = INSTRUCTION_SET_HEADER;
    headerPrefix = HEADER_PREFIX;
    commentPrefix = COMMENT_PREFIX;
    delimiter = DELIMITER;
}

int InstructionSetModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return encodingConfig->opcodeCount();
}

int InstructionSetModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

QVariant InstructionSetModel::headerData(int section,
                                         Qt::Orientation orientation,
                                         int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return {};

    switch (section) {
    case OpcodeColumn:   return "Opcode";
    case MnemonicColumn: return "Mnemonic";
    case TypeColumn:     return "Type";
    case FormatColumn:   return "Format";
    default:             return {};
    }
}

Qt::ItemFlags InstructionSetModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    if (index.column() == OpcodeColumn)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    // protect NOP definition
    if (index.row() == 0){
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

QVariant InstructionSetModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return {};

    const auto def = instructionSet->getDefinition(index.row());

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case OpcodeColumn:
            return index.row();
        case MnemonicColumn:
            if(def == nullptr) return {};
            return def->mnemonic;
        case TypeColumn:
            if(def == nullptr) return {};
            return typeToString(def->type);
        case FormatColumn:
            if(def == nullptr) return {};
            return def->getFormatString();
        }
    }

    return {};
}

bool InstructionSetModel::setData(const QModelIndex& index,
                                  const QVariant& value,
                                  int role)
{
    if (role != Qt::EditRole || !index.isValid())
        return false;

    const auto def = instructionSet->getDefinition(index.row());
    if(def == nullptr){
        return false;
    }

    InstructionDefinition newDef = *def;

    switch (index.column()) {
    case MnemonicColumn:
        newDef.mnemonic = value.toString();
        break;

    case TypeColumn: {
        bool ok;
        Assembly::InstructionType t = stringToType(value.toString(), &ok);
        if(!ok) return false;
        newDef.type = t;
        break;
    }

    case FormatColumn:
        newDef = InstructionDefinition(newDef.mnemonic, newDef.type, value.toString());
        //def.formatTokens =;
        break;

    default:
        return false;
    }

    if(*def != newDef){
        instructionSet->setDefinition(index.row(), newDef);
        emit instructionSetChanged();
    }

    return true;
}

void InstructionSetModel::clear(){
    beginResetModel();
    for(qsizetype opcode = 1; opcode<rowCount(); opcode++){
        instructionSet->removeDefinition(opcode);
    }
    endResetModel();
}

void InstructionSetModel::populateFromStringMatrix(const QList<QList<QString>> &rows){
    clear();
    beginResetModel();
    for(qsizetype row = 1; row<rows.size(); row++){
        if(rows[row].size() == ColumnCount){
            QString mnemonic = rows[row][MnemonicColumn];
            if(instructionSet->getDefinition(mnemonic) != nullptr){
                continue;
            }
            bool ok;
            InstructionType type = stringToType(rows[row][TypeColumn], &ok);
            if(!ok || type == InstructionType::None){
                continue;
            }

            instructionSet->setDefinition(row, {mnemonic, type, rows[row][FormatColumn]});
        }
    }

    endResetModel();
}

bool InstructionSetModel::isRowEmpty(const qsizetype row) const{
    auto def = instructionSet->getDefinition(row);
    if(def == nullptr) return true;
    return def->type == InstructionType::None || def->mnemonic.isEmpty();
}

QString InstructionSetModel::typeToString(Assembly::InstructionType type){
    switch(type){
        case Assembly::InstructionType::R: return "R";
        case Assembly::InstructionType::I: return "I";
        case Assembly::InstructionType::J: return "J";
        case Assembly::InstructionType::None: return "";
    }
    return "";
}

Assembly::InstructionType InstructionSetModel::stringToType(const QString& str, bool* okptr){
    if(str.isEmpty()){
        if(okptr != nullptr) *okptr = true;
        return Assembly::InstructionType::None;
    }
    else if(str.compare("R", Qt::CaseInsensitive) == 0){
        if(okptr != nullptr) *okptr = true;
        return Assembly::InstructionType::R;
    }
    else if(str.compare("I", Qt::CaseInsensitive) == 0){
        if(okptr != nullptr) *okptr = true;
        return Assembly::InstructionType::I;
    }
    else if(str.compare("J", Qt::CaseInsensitive) == 0){
        if(okptr != nullptr) *okptr = true;
        return Assembly::InstructionType::J;
    }

    if(okptr != nullptr) *okptr = false;
    return Assembly::InstructionType::None;
}


} // namespace Models
