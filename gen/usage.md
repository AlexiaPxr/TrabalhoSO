# Small tests example

```bash
python3 gen.py -s 5 --min_req 50 --max_req 100 -p 32 > entrada_media.txt
```

# Medium tests example

```bash
$ python3 gen.py -s 10 --min_req 100 --max_req 1000 -p 128 --mfisica 131072 --mvirtual 262144 > medium.txt     
```

# Large tests example

```bash
$ python3 gen.py -s 10 --min_req 1000 --max_req 10000 -p 32768 --mfisica 2097152 --mvirtual 8388608 > large.txt
```