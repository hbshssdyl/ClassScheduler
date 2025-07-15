from fastapi import FastAPI
from pydantic import BaseModel

app = FastAPI()

class User(BaseModel):
    username: str
    password: str

@app.post("/login")
def login(user: User):
    return {"message": f"Welcome, {user.username}!"}
