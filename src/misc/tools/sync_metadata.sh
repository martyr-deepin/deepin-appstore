wget https://dstore-metadata.deepin.cn/api/app -O app.json 
go build -o metadata_sync .
./metadata_sync
