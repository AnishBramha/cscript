
def declareType(writer, baseName: str, className: str, fieldList: str, allBaseNames : list[str]) -> None:

    writer.write(f'class {className} : public {baseName} {{\n\n')
    writer.write('\tpublic:\n\n')

    fields = [field.strip() for field in fieldList.split(',')] if fieldList else []

    for field in fields:

        type_name, name = field.rsplit(' ', 1)

        if type_name in allBaseNames and type_name != 'Block':
            writer.write(f'\t\tconst std::unique_ptr<{type_name}> {name};\n')

        else:
            writer.write(f'\t\tconst {type_name} {name};\n')

    writer.write('\n')
    writer.write(f'\t\t{className}(')

    paramList = []

    for field in fields:

        type_name, name = field.rsplit(' ', 1)

        if type_name in allBaseNames and type_name != 'Block':
            paramList.append(f'std::unique_ptr<{type_name}> {name}')

        elif 'vector' in type_name:
            paramList.append(f'{type_name}&& {name}')

        else:
            paramList.append(f'const {type_name}& {name}')

    writer.write(', '.join(paramList))
    writer.write(');\n\n')
    writer.write('\t\tobject accept(Visitor& visitor) override;\n\n')
    writer.write('};\n\n\n')





def defineType(writer, baseName: str, className: str, fieldList: str, allBaseNames : list[str]) -> None:

    writer.write(f'{className}::{className}(')
    
    paramList = []

    fields = [field.strip() for field in fieldList.split(',')] if fieldList else []

    for field in fields:

        type_name, name = field.rsplit(' ', 1)

        if type_name in allBaseNames and type_name != 'Block':
            paramList.append(f'std::unique_ptr<{type_name}> {name}')

        elif 'vector' in type_name:
            paramList.append(f'{type_name}&& {name}')

        else:
            paramList.append(f'const {type_name}& {name}')

    writer.write(', '.join(paramList))
    writer.write(') : ')

    initList = []

    for field in fields:

        type_name, name = field.rsplit(' ', 1)

        if type_name in allBaseNames or 'vector' in type_name:
            initList.append(f'{name}(std::move({name}))')

        else:
            initList.append(f'{name}({name})')

    writer.write(', '.join(initList))
    writer.write(' {}\n\n')
    writer.write(f'object {className}::accept(Visitor& visitor) {{\n\n')
    writer.write(f'\treturn visitor.visit{className}{baseName}(*this);\n')
    writer.write('}\n\n\n')






def defineAst(outputDir: str, baseName: str, types: list[str], allBaseNames : list[str]) -> None:

    hpp_path = f"{outputDir}/{baseName}.hpp"

    with open(hpp_path, 'w') as writer:

        writer.write('#pragma once\n\n')
        writer.write('#include <memory>\n')
        writer.write('#include "../tokeniser/Token.hpp"\n')

        if hpp_path.endswith('Stmt.hpp') or hpp_path.endswith('Stmt.cpp'):
            writer.write('#include "./Expr.hpp"\n')

        writer.write('#include "../superclass/super.hpp"\n')
        writer.write('#include "./Visitor.hpp"\n\n')
        writer.write('using super::object;\n\n\n')

        writer.write(f'class {baseName} {{\n\n')
        writer.write('\tpublic:\n\n')
        writer.write('\t\tvirtual object accept(Visitor& visitor) = 0;\n\n')
        writer.write(f'\t\tvirtual ~{baseName}() = default;\n')
        writer.write('};\n\n\n')
       
        for _type in types:

            className = _type.split(':')[0].strip()
            fields = _type.split(':')[1].strip()

            declareType(writer, baseName, className, fields, allBaseNames)

        for _ in range(5):
            writer.write('\n')


    cpp_path = f'{outputDir}/{baseName}.cpp'

    with open(cpp_path, 'w') as writer:

        writer.write('#include "../superclass/super.hpp"\n\n')
        writer.write(f'#include "./{baseName}.hpp"\n\n')
        writer.write('using namespace std;\n')
        writer.write('using super::object;\n\n\n')

        for _type in types:

            className = _type.split(':')[0].strip()
            fields = _type.split(':')[1].strip()

            defineType(writer, baseName, className, fields, allBaseNames)




def declareVisitor(outputDir : str, baseNames : dict[str, list[str]]) -> None:

    path = f'{outputDir}/Visitor.hpp'

    with open(path, 'w') as writer:

        writer.write('#pragma once\n\n')
        writer.write('#include "../superclass/super.hpp"\n\n')
        writer.write('#include <vector>\n\n')
        writer.write('#include <memory>\n\n')
        writer.write('using super::object;\n\n')
        writer.write('using std::vector;\n\n')
        writer.write('using std::unique_ptr;\n\n')

        for baseName, types in baseNames.items():

            for _type in types:

                  className = _type.split(':')[0].strip()
                  writer.write(f'class {className};\n')

        writer.write('\n\n')
        writer.write('class Visitor {\n\n')
        writer.write('\tpublic:\n\n')
        
        for baseName, types in baseNames.items():

            for _type in types:

                typeName = _type.split(':')[0].strip()
                writer.write(f'\t\tvirtual object visit{typeName}{baseName}(')
                writer.write(f'const {typeName}& {baseName.lower()});\n')

            writer.write('\n')

        writer.write('\n\t\tvirtual ~Visitor() = default;\n')
        writer.write('};\n\n\n\n\n')




def defineVisitor(outputDir : str, baseNames : dict[str, list[str]]) -> None:

    declareVisitor(outputDir, baseNames)

    cpp_path = f'{outputDir}/Visitor.cpp'

    with open(cpp_path, 'w') as writer:

        writer.write('#include "./Visitor.hpp"\n')
        writer.write('#include "./Expr.hpp"\n')
        writer.write('#include "./Stmt.hpp"\n\n')
        writer.write('#include <vector>\n\n')
        writer.write('#include <memory>\n\n')
        writer.write('using super::object;\n\n')
        writer.write('using std::vector;\n\n')
        writer.write('using std::unique_ptr;\n\n')

        for baseName, types in baseNames.items():

            for _type in types:

                typeName = _type.split(':')[0].strip()
                writer.write(f'object Visitor::visit{typeName}{baseName}(')
                writer.write(f'const {typeName}& {baseName.lower()}) {{\n\n')
                writer.write(f'\treturn object();\n')
                writer.write(f'}}\n\n')





if __name__ == '__main__':

    exprTypes = [

        'Assign     : Token name, Expr value',
        'Binary     : Expr left, Token oprtor, Expr right',
        'Call       : Expr callee, Token paren, vector<unique_ptr<Expr>> args',
        'Grouping   : Expr expr',
        'Literal    : object value',
        'Logical    : Expr left, Token operatr, Expr right',
        'Unary      : Token oprtor, Expr right',
        'Variable   : Token name'
    ]

    stmtTypes = [

        'Block      : vector<unique_ptr<Stmt>> statements',
        'Expression : Expr expr',
        'Function   : Token name, vector<Token> params, vector<unique_ptr<Stmt>> body',
        'If         : Expr condition, Stmt thenBranch, Stmt elseBranch',
        'Print      : Expr expr',
        'Println    : Expr expr',
        'Return     : Token keyword, Expr value',
        'Var        : Token name, Expr initialiser',
        'While      : Expr condition, Stmt body'
    ]

    allTypes = {

        'Expr': exprTypes,
        'Stmt': stmtTypes
    }

    allBaseNames = list(allTypes.keys())

    outputDir = '../lexer'


    defineVisitor(outputDir, allTypes)
    defineAst(outputDir, 'Expr', exprTypes, allBaseNames)
    defineAst(outputDir, 'Stmt', stmtTypes, allBaseNames)










