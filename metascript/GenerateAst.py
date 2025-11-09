def declareType(writer, baseName: str, className: str, fieldList: str) -> None:

    writer.write(f'class {className} : public {baseName} {{\n\n')

    writer.write('\tpublic:\n\n')

    fields = [field.strip() for field in fieldList.split(',')]

    for field in fields:

        type_name, name = field.split()

        if type_name == baseName:

            writer.write(f'\t\tconst std::unique_ptr<{type_name}> {name};\n')

        else:

            writer.write(f'\t\tconst {type_name} {name};\n')

    writer.write('\n')

    writer.write(f'\t\t{className}(')

    paramList = []

    for field in fields:

        type_name, name = field.split()

        if type_name == baseName:

            paramList.append(f'std::unique_ptr<{type_name}> {name}')

        else:

            paramList.append(f"const {type_name}& {name}")

    writer.write(', '.join(paramList))

    writer.write(');\n\n')

    writer.write('\t\tobject accept(Visitor& visitor) override;\n\n')

    writer.write('};\n\n\n')



def defineType(writer, baseName: str, className: str, fieldList: str) -> None:

    writer.write(f'{className}::{className}(')
    
    paramList = []

    fields = [field.strip() for field in fieldList.split(',')]

    for field in fields:

        type_name, name = field.split()

        if type_name == baseName:

            paramList.append(f'std::unique_ptr<{type_name}> {name}')

        else:

            paramList.append(f'const {type_name}& {name}')

    writer.write(', '.join(paramList))

    writer.write(') : ')

    initList = []

    for field in fields:

        type_name = field.split()[0]
        name = field.split()[1]

        if type_name == baseName:

            initList.append(f'{name}(std::move({name}))')

        else:

            initList.append(f'{name}({name})')

    writer.write(', '.join(initList))

    writer.write(' {}\n\n')

    writer.write(f'object {className}::accept(Visitor& visitor) {{\n\n')

    writer.write(f'\treturn visitor.visit{className}{baseName}(*this);\n')

    writer.write('}\n\n\n')



def defineAst(outputDir: str, baseName: str, types: list[str]) -> None:

    hpp_path = f"{outputDir}/{baseName}.hpp"

    with open(hpp_path, 'w') as writer:

        writer.write('#pragma once\n\n')

        writer.write('#include <memory>\n')
        writer.write('#include "../tokeniser/Token.hpp"\n\n')
        writer.write('#include "../superclass/super.hpp"\n\n')
       
        typeNames = [_type.split(':')[0].strip() for _type in types]

        for typeName in typeNames:

            writer.write(f'class {typeName};\n')

        writer.write('class Visitor;\n\n') 

        writer.write(f'class {baseName} {{\n\n')

        writer.write('\tpublic:\n\n')


        writer.write('\t\tvirtual object accept(Visitor& visitor) = 0;\n\n')

        writer.write('\t\tvirtual ~Expr() = default;\n');

        writer.write('};\n\n\n')

        declareVisitor(writer, baseName, types)

        for _type in types:

            className = _type.split(':')[0].strip()

            fields = _type.split(':')[1].strip()

            declareType(writer, baseName, className, fields)

        
        for _ in range(5):
            writer.write('\n')


    cpp_path = f'{outputDir}/{baseName}.cpp'

    with open(cpp_path, 'w') as writer:

        writer.write('#include "../superclass/super.hpp"\n\n')

        writer.write(f'#include "./{baseName}.hpp"\n\n')

        writer.write('using namespace std;\n\n')

        for _type in types:

            className = _type.split(':')[0].strip()

            fields = _type.split(':')[1].strip()

            defineType(writer, baseName, className, fields)



def declareVisitor(writer, baseName: str, types: list[str]):

    writer.write('class Visitor {\n\n')

    writer.write('\tpublic:\n\n')

    for _type in types:

        typeName = _type.split(':')[0].strip()

        writer.write(f'\t\tvirtual object visit{typeName}{baseName}(')

        writer.write(f'const {typeName}& {baseName.lower()}) = 0;\n')

    writer.write('\n\t\tvirtual ~Visitor() = default;\n')

    writer.write('};\n\n\n')


if __name__ == '__main__':

    defineAst('.', 'Expr', [

        'Binary   : Expr left, Token oprtor, Expr right',
        'Grouping : Expr expr',
        'Literal  : object value',
        'Unary    : Token oprtor, Expr right'
    ])











