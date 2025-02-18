# OBJ files

My program has so far allocated everything on the stack for multiple efficiency.  
If I want to handle obj file with 20000 triangles then the stack size becomes an issue.

I can check the size of my stack with 
```bash
ulimit -s
```

I get 8192 KB
A triangle struct is 344 Bytes.  
If I allocate 10000 triangles I get 3.440.000 Bytes or 3440 KB. It is already half of my stack
