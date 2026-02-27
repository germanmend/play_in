from flask import Flask, redirect, request, jsonify
import requests

app = Flask(__name__)

# CONFIGURATION (From Riot Portal)
CLIENT_ID = "YOUR_CLIENT_ID"
CLIENT_SECRET = "YOUR_CLIENT_SECRET"
REDIRECT_URI = "https://your-ngrok-url.ngrok-free.dev/callback"

# Simple database to store user tokens (use a real DB for production)
user_store = {}

@app.route('/login')
def login():
    # Send user to Riot to log in
    riot_auth_url = (
        f"https://auth.riotgames.com/authorize?client_id={CLIENT_ID}"
        f"&redirect_uri={REDIRECT_URI}&response_type=code&scope=openid+offline_access"
    )
    return redirect(riot_auth_url)

@app.route('/callback')
def callback():
    # Riot sends the user back here with a 'code'
    code = request.args.get('code')

    # Exchange code for an Access Token
    token_url = "https://auth.riotgames.com/token"
    data = {
        "grant_type": "authorization_code",
        "code": code,
        "redirect_uri": REDIRECT_URI
    }
    # Use Basic Auth with ClientID:ClientSecret
    response = requests.post(token_url, data=data, auth=(CLIENT_ID, CLIENT_SECRET)).json()

    access_token = response['access_token']

    # Get the user's Summoner Name to identify them
    user_info = requests.get(
        "https://na1.api.riotgames.com/lol/summoner/v4/summoners/me",
        headers={"Authorization": f"Bearer {access_token}"}
    ).json()

    summoner_name = user_info['name']
    user_store[summoner_name] = access_token # Save for the ESP32 to fetch

    return f"Success! {summoner_name} is now linked to your ESP32."

@app.route('/api/profile/<name>')
def get_profile(name):
    # ESP32 calls this to get stats for a specific logged-in friend
    token = user_store.get(name)
    if not token: return jsonify({"error": "User not logged in"}), 404

    # Fetch real stats using the friend's Token
    # ... (Same logic as before, but use Bearer Token instead of API Key) ...
    return jsonify({"nick": name, "status": "Authenticated"})

if __name__ == '__main__':
    app.run(port=5000)
