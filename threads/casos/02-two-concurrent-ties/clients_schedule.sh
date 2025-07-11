echo 'crear PartidaImpares' >> __client_1_pipe__
echo 'crear PartidaPares' >> __client_2_pipe__
sleep 1
echo 'jugar 2 2' >> __client_1_pipe__
echo 'jugar 1 1' >> __client_2_pipe__
echo 'listar' >> __client_3_pipe__
echo 'listar' >> __client_4_pipe__
echo 'unirse PartidaPares' >> __client_4_pipe__
echo 'unirse PartidaImpares' >> __client_3_pipe__

echo 'jugar 3 3' >> __client_3_pipe__
echo 'jugar 3 1' >> __client_1_pipe__

echo 'jugar 2 2' >> __client_4_pipe__
echo 'jugar 3 3' >> __client_2_pipe__

echo 'jugar 1 3' >> __client_3_pipe__
echo 'jugar 2 3' >> __client_1_pipe__
echo 'jugar 2 1' >> __client_3_pipe__

echo 'jugar 2 1' >> __client_2_pipe__
echo 'jugar 2 3' >> __client_4_pipe__

echo 'jugar 3 1' >> __client_4_pipe__
echo 'jugar 1 3' >> __client_2_pipe__
echo 'jugar 1 2' >> __client_4_pipe__
echo 'jugar 3 2' >> __client_2_pipe__

echo 'jugar 3 2' >> __client_1_pipe__
echo 'jugar 1 2' >> __client_3_pipe__
echo 'jugar 1 1' >> __client_1_pipe__
