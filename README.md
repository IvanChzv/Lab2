# Lab2
Для начала выберите папку, в которой вы планируете разместить программу. Затем откройте ее в терминале и пропишите комманду

`git init`

Затем скачайте файлы из репозитория github, используя комманду:

`git clone https://github.com/IvanChzv/Lab2.git`

После этого у Вас появится папка Lab2 со всем необходимым для работы. Откройте ее в теримане и пропишите:

`bash bash.sh`

Данная комманда скомпилирует программы в папку build. В ней находятся два испольняемых файла: client и server.

Первым делом запустим сервер. Для этого откроем папку build в терминале и пропишем комманду:

`./server <port> <max threads> <max file size> <save directory>`

Где `port` - порт подключения,

`max threads` - максимальное кол-во потоков для обработки,

`max file size` - максимальный размер файла,

`save directory` - директория сохранения файла.

Если все параметры введены верно, то в терминале высветится сообщение о старте сервера.

Теперь запустим клиент и отправим файл. Для этого в новом терминале откроем папку build и пропишем комманду:

`./client <directory> <filename> <port>`

Где `directory` - директория файла,

`filename` - имя файла,

`port` - порт подключения.

Если файл отправлен успешно, то терминал сервера и клиента сообщат соответственно об отправке и получении данных.
