import sys
import numpy as np
import os

# First argument is the number of states
numStates = int(sys.argv[1])
print(("Number of states = %d" % (numStates)))

# Second argument is the connection probability
connProb = float(sys.argv[2])
print(("Connection probability = %f" % (connProb)))

# Third argument is the final state probability
finalProb = float(sys.argv[3])
print(("Final state probability = %f" % (finalProb)))

# Fourth argument is whether the automaton has lambda transitions (1) or not (0)
has_lambda = True if sys.argv[4] == "True" else False
if has_lambda:
    print("WITH-LAMBDA")
else:
    print("WITHOUT-LAMBDA")

# Fifth argument is whether or not to make and execute C code:
execute = True if sys.argv[5] == "True" else False

# Sixth argument is the number of symbols in the input string:
num_input_symbols = int(sys.argv[6])

# The rest of arguments are the input symbols
inputSymbols = sys.argv[7:]
if has_lambda:
    inputSymbols.append("&#955;")
print("List of input symbols:")
print(inputSymbols)
numSymbols = len(inputSymbols)

# Final states, the last state is always final:
final = np.random.random((numStates,1)) < finalProb
final[-1] = True

# Generate the automaton:
table = np.random.random((numStates,numStates,numSymbols)) < connProb

# Generate random input string:
rnum = np.random.randint(0, numSymbols, num_input_symbols)
inputString = [inputSymbols[x] for x in rnum]
print("Input string =", inputString)

#---------------------------------------------------------------------------------
# Print to file in C format:
#---------------------------------------------------------------------------------
f = open("automaton.c", "w")

f.write("#include <stdio.h>\n")
f.write("#include <stdlib.h>\n")
f.write("#include <string.h>\n")
f.write('#include "afnd.h"\n')
f.write("\n")

f.write("int main() {\n")
f.write("  AFND *p;\n")
f.write("\n")

f.write("  // Creo automata:\n")
f.write('  p = AFNDNuevo("automata1", %d, %d);\n' % (numStates, numSymbols))
f.write("\n")

f.write("  // Inserto alfabeto:\n")
for s in inputSymbols:
    f.write('  if (!AFNDInsertaSimbolo(p, "%s")) return 1;\n' % (s))
f.write("\n")

f.write("  // Inserto estados:\n")
for i, fs in enumerate(final):
    f.write('  if (!AFNDInsertaEstado(p, "q%d"' % (i))
    if i == 0 and fs:
        f.write(', INICIAL_Y_FINAL)) return 1;\n')
    elif i == 0:
        f.write(', INICIAL)) return 1;\n')
    elif fs:
        f.write(', FINAL)) return 1;\n')
    else:
        f.write(', NORMAL)) return 1;\n')
f.write("\n")

f.write("  // Inserto transiciones:\n")
for i in range(0, numStates):
    for j in range(0, numStates):
        for k in range(0, numSymbols):
            if table[i][j][k]:
                f.write('  if (!AFNDInsertaTransicion(p, "q%d", "%s", "q%d")) return 1;\n'
                        % (i, inputSymbols[k], j))
f.write("\n")

f.write("  // Imprimo el automata:\n")
f.write("  AFNDImprime(stdout, p);\n")
f.write("\n")

f.write("  // Inserto cadena de entrada:\n")
for x in inputString:
    f.write('  if (!AFNDInsertaLetra(p, "%s")) return 1;\n' % (x))
f.write("\n")

f.write("  // Imprimo cadena:\n")
f.write("  AFNDImprimeCadenaActual(stdout, p);\n")
f.write("\n")

f.write("  // Inicializo el automata:\n")
f.write("  if (!AFNDInicializaEstado(p)) return 1;\n")
f.write("\n")

f.write("  // Proceso la cadena con el automata:\n")
f.write("  AFNDProcesaEntrada(stdout, p);\n")
f.write("\n")

f.write("  // Libero memoria:\n")
f.write("  AFNDElimina(p);\n")
f.write("\n")

f.write("  return 0;\n")
f.write("}\n")

f.close()

#---------------------------------------------------------------------------------
# Print to file in dot format:
#---------------------------------------------------------------------------------
f = open("automaton.dot", "w")
f.write("// How to run dot: dot -Tpng xxx.dot -o xxx.png\n");
f.write("digraph {\n")
f.write("  rankdir=LR;\n")
f.write("\n")
f.write("  start [style=invis];\n")
for i in range(0, numStates):
    if final[i]:
        f.write("  q%d[label=<q<sub>%d</sub>>, shape=\"doublecircle\", color=\"black\"];\n" % (i,i))
    else:
        f.write("  q%d[label=<q<sub>%d</sub>>, shape=\"circle\", color=\"black\"];\n" % (i,i))

f.write("\n")
f.write("  start -> q0[color=\"black\"];\n")

for i in range(0, numStates):
    for j in range(0, numStates):
        # Connections from state i to state j:
        connections = table[i][j]

        if sum(connections) > 0:
            f.write("  q%d -> q%d[label=\"" % (i,j))
            isFirst = True
            for k in range(0, numSymbols):
                if connections[k]:
                    if isFirst:
                        f.write("%s" % (inputSymbols[k]))
                        isFirst = False
                    else:
                        f.write(",%s" % (inputSymbols[k]))
            f.write("\", color=\"black\", fontcolor=\"black\"];\n")

f.write("}\n")

# Close open file
f.close()

# Generate plot:
os.system("dot -Tpng automaton.dot -o automaton.png")
os.system("eog automaton.png&")

