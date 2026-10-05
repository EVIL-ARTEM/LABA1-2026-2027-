#!/bin/bash
# Контрольный пример: запускает команды по очереди и показывает результат.
# Сборка: g++ -std=c++17 *.cpp -o dbms
F=test.data
rm -f $F
run() { echo "\$ $1"; ./dbms --file $F --query "$1"; }

echo "===== АВЛ-дерево ====="
for k in 10 20 30 40 50 25; do run "TINSERT mytree $k"; done
run "PRINT mytree"
run "TINORDER mytree"; run "TPREORDER mytree"; run "TPOSTORDER mytree"; run "TBFS mytree"
run "ISMEMBER mytree 25"; run "TGET mytree 99"
run "TDEL mytree 30"; run "PRINT mytree"

echo "===== Массив ====="
run "MPUSH arr x"; run "MPUSH arr y"; run "MINSERT arr 1 z"; run "PRINT arr"
run "MSET arr 0 X"; run "MGET arr 1"; run "MDEL arr 2"; run "MLEN arr"; run "PRINT arr"

echo "===== Односвязный список ====="
run "FPUSHHEAD fl a"; run "FPUSHTAIL fl d"
run "FPUSHBEFORE fl d c"; run "FPUSHAFTER fl d e"; run "PRINT fl"; run "FPRINTREV fl"
run "FGET fl 2"; run "FFIND fl c"; run "FDELBEFORE fl c"; run "FDELAFTER fl d"
run "FDELHEAD fl"; run "FDELTAIL fl"; run "FDEL fl c"; run "PRINT fl"

echo "===== Двусвязный список ====="
run "LPUSH dl b"; run "LPUSHHEAD dl a"; run "LPUSHTAIL dl d"
run "LPUSHBEFORE dl d c"; run "LPUSHAFTER dl d e"; run "PRINT dl"; run "LPRINTREV dl"
run "LDELBEFORE dl c"; run "LDELAFTER dl d"; run "LDELTAIL dl"; run "LDEL dl c"; run "PRINT dl"

echo "===== Стек ====="
run "SPUSH mystack a"; run "SPUSH mystack b"; run "SPUSH mystack c"; run "PRINT mystack"; run "SPOP mystack"

echo "===== Очередь ====="
run "QPUSH myqueue 1"; run "QPUSH myqueue 2"; run "QPUSH myqueue 3"; run "QPOP myqueue"; run "PRINT myqueue"

echo "===== Содержимое файла $F ====="
cat $F