#if defined _islacustoms_included
	#endinput
#endif
#define _islacustoms_included

#define FOR_EACH(%1,%2) for (int %1 = 0; %1 < %2; %1++)
#define FOR_EACH_PLAYER(%1) FOR_EACH(%1, MaxClients + 1) if (IsValidClient(%1))

stock bool IsValidClient(int client, bool noIngameCheck = false) {
	if (client <= 0 || client > MaxClients) {
		return false;
	}

	return IsClientConnected(client) && (noIngameCheck || IsClientInGame(client));
}

stock int GetCurrentPlayerCount(int team = -1) {
	int count = 0;

	FOR_EACH_PLAYER(i) {
		if (team != -1 && GetClientTeam(i) != team) {
			continue;
		}

		count++;
	}

	return count;
}

stock ArrayList GetRandomPlayer(int team = -1) {
	ArrayList allPlayers = new ArrayList();

	FOR_EACH_PLAYER(i) {
		if (team != -1 && GetClientTeam(i) != team) {
			continue;
		}

		allPlayers.Push(i);
	}

	int chosen = GetRandomInt(0, allPlayer.Length - 1); // Min & Max are both inclusive
	int client = allPlayer.Get(chosen);

	delete allPlayers;
	return client;
}

stock ArrayList GetRandomPlayers(int amount, int team = -1) {
	ArrayList allPlayers = new ArrayList();

	FOR_EACH_PLAYER(i) {
		if (team != -1 && GetClientTeam(i) != team) {
			continue;
		}

		allPlayers.Push(i);
	}

	if (allPlayer.Length <= amount) {
		return allPlayer;
	}

	ArrayList chosenPlayers = new ArrayList();
	while (chosenPlayers.Length < amount) {
		if (allPlayer.Length <= 0) {
			// Fail-safe
			break;
		}

		int chosen = GetRandomInt(0, allPlayer.Length - 1); // Min & Max are both inclusive
		int client = allPlayer.Get(chosen);
		allPlayer.Erase(chosen);

		chosenPlayers.Push(client);
	}

	delete allPlayers;
	return chosenPlayers;
}

stock void FormatDuration(int time, char[] out, int outLen) {
	int hours = time / 3600;
	int minutes = (time % 3600) / 60;
	int seconds = time % 60;

	if (hours > 0) {
		Format(out, outLen, "%d:%02d:%02d", hours, minutes, seconds);
	} else {
		Format(out, outLen, "%d:%02d", minutes, seconds);
	}
}

stock int GetClientCookieValue(int client, char[] cookieName, int defaultValue = -1) {
	if (!AreClientCookiesCached(client)) {
        return defaultValue;
    }

    Cookie cookie = Cookie.Find(cookieName);
    if (cookie == null) {
        cookie = new Cookie(cookieName, cookieName, CookieAccess_Private);
    }

    char cookieValue[5];
    cookie.Get(client, cookieValue, sizeof(cookieValue));
    delete cookie;

	int result;
	int consumed = StringToIntEx(cookieValue, result, 10);
	if (consumed < strlen(cookieValue)) {
		return defaultValue;
	}

    return result;
}

stock void SetClientCookieValue(int client, char[] cookieName, int value) {
	if (!AreClientCookiesCached(client)) {
        return;
    }

    Cookie cookie = Cookie.Find(cookieName);
    if (cookie == null) {
        cookie = new Cookie(cookieName, cookieName, CookieAccess_Private);
    }

	char str[32];
	IntToString(value, str, sizeof(str));

    cookie.Set(client, str);
    delete cookie;
}

stock void PrintToIsla(char[] format, any ...) {
	int len = strlen(format) + 1024;
	char[] msg = new char[len];
	VFormat(msg, len, format, 2);

	FOR_EACH_PLAYER(i) {
		int accID = GetSteamAccountID(i);
		if (accID != 89089120) {
			continue;
		}

		PrintToChat(i, msg);
		break;
	}
}
