#include "GameScene.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "ParticleManager.h"
#include "AudioManager.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "MyTools.h"
#ifdef _DEBUG
#include "DebugLineBase.h"
#endif // _DEBUG
#include <curl/curl.h>

using json = nlohmann::json;
using namespace std;
using namespace MyBase;

// 初期化
void GameScene::Initialize() {
#pragma region シーン初期化
	BaseScene::Initialize();

#pragma region ライト
	LightManager::GetInstance()->SetPointLightIntensity(0.0f);
	LightManager::GetInstance()->SetDirectionalLightIntensity(1.0f);
	LightManager::GetInstance()->SetSpotLightIntensity(0.0f);
#pragma endregion ライト

#pragma region スプライト
	
#pragma endregion スプライト

#pragma region 3Dオブジェクト

#pragma endregion 3Dオブジェクト

#pragma region カメラ

#pragma endregion カメラ

#pragma region デバッグ
#ifdef _DEBUG
	DebugLineBase::GetInstance()->Initialize(DirectXBase::GetInstance());
#endif // _DEBUG
#pragma endregion

#pragma region ロジック
	logicType_ = LogicName::LOGIN;
#pragma endregion ロジック


#pragma region パーティクル
	// パーティクル
	
#pragma endregion パーティクル

#pragma region jsonローダー
	// jsonローダー
	jsonLoader_ = make_unique<JsonLoader>();
	LoadJsonFile("gameScene.json");
#pragma endregion jsonローダー

#pragma region オーディオ
	// BGM

	// お試し用
	AudioManager::GetInstance()->LoadAudioWave("fanfare.wav");
#pragma endregion オーディオ

	loginNameBuffer_[0] = '\0';
	loginPasswordBuffer_[0] = '\0';
	rankingText_.clear();

	// 最初の更新
	CameraManager::GetInstance()->GetCamera()->SetTranslate(kCameraTranslate);
	CameraManager::GetInstance()->GetCamera()->Update();

	curl_global_init(CURL_GLOBAL_DEFAULT);
#pragma endregion シーン初期化
}

// 終了
void GameScene::Finalize() {
	jsonLoader_.reset();

	// 3Dオブジェクト
	

	// スプライト
	
	
#ifdef _DEBUG
	DebugLineBase::Finalize();
#endif // _DEBUG

	BaseScene::Finalize();

	curl_global_cleanup();
}

// 毎フレーム更新
void GameScene::Update()
{
	curl_global_init(CURL_GLOBAL_DEFAULT);

	BaseScene::Update();

#ifdef _DEBUG
	// デバッグ更新
	DebugUpdate();
#endif // _DEBUG

	// カメラマネージャーの更新
	CameraManager::GetInstance()->Update(TimeManager::GetInstance()->GetDeltaTime());
	// カメラの更新
	if (CameraManager::GetInstance()->GetCamera()) {
		CameraManager::GetInstance()->GetCamera()->Update();
	}

	// 3Dオブジェクトの更新処理

	// ロジックの更新処理
	switch (logicType_) {
		case LogicName::LOGIN:
			break;
		case LogicName::WAIT_START:
			
			if (input_->TriggerKey(DIK_SPACE)) {
				logicType_ = LogicName::RUNNING;

				sec_ = 0.0;
				score_ = 0;
				start_ = chrono::steady_clock::now();

				break;
			}

			break;
		case LogicName::RUNNING:
			if (input_->TriggerKey(DIK_SPACE)) {
				logicType_ = LogicName::RESULT;

				double diff = abs(sec_ - 10.0);
				score_ = (sec_ <= 10.0) ? max(0, 1000 - (int)(diff * 100)) : 0;

				break;
			}

			// 計測中の処理
			end_ = chrono::steady_clock::now();
			sec_ = chrono::duration<double>(end_ - start_).count();

			break;
		case LogicName::RESULT:
			if (input_->TriggerKey(DIK_RETURN)) {
				logicType_ = LogicName::RANKING;

				// スコア取得
				string postRes = PostScoreAsync(score_, token_).get();
				string allScoresJson = GetAllScoresAsync(token_).get();

				OutputDebugStringA(postRes.c_str());
				OutputDebugStringA(allScoresJson.c_str());

				try {
					json j = json::parse(allScoresJson);
					rankingText_ = "Ranking Top 5:\n";
					int i = 0;
					for (const auto& entry : j) {
						int rankScore = entry["score"];
						rankingText_ += to_string(i + 1) + ". " + to_string(rankScore) + "\n";
						++i;
					}
				}
				catch (const json::parse_error& e) {
					rankingText_ = "[Ranking Error]\n";
					rankingText_ += e.what();
				}

				break;
			}
			break;
		case LogicName::RANKING:
			if (input_->TriggerKey(DIK_SPACE)) {
				logicType_ = LogicName::WAIT_START;

				break;
			}
			break;
		default:
			break;
	}

	// パーティクルの更新処理
	ParticleManager::GetInstance()->Update();

	// スプライトの更新処理
	

	curl_global_cleanup();
}

// 描画
void GameScene::Draw() {
#pragma region 3Dオブジェクト

	// 3Dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	ModelManager::GetInstance()->SetCommonScreen();

	// 全ての3DObject個々の描画


#pragma endregion 3Dオブジェクト

#pragma region パーティクル

	// パーティクルの描画準備。パーティクルの描画に共通グラフィックスコマンドを積む
	ParticleManager::GetInstance()->Draw();

#pragma endregion パーティクル

#pragma region スプライト

	// Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
	TextureManager::GetInstance()->SetCommonScreen();

	// 全てのSprite個々の描画
	

#pragma endregion スプライト
}

#ifdef _DEBUG
// デバッグ更新
void GameScene::DebugUpdate() {
	DebugDraw();
}

// デバッグ描画
void GameScene::DebugDraw() {
	// 開発用UIの処理。実際に開発用のUIを出す場合はここをゲーム固有の処理に置き換える
	ImGui::SetNextWindowPos(kDebugWindowPosGame, ImGuiCond_Once);		// ウィンドウの座標(プログラム起動時のみ読み込み)
	ImGui::SetNextWindowSize(kDebugWindowSizeGame, ImGuiCond_Once);		// ウィンドウのサイズ(プログラム起動時のみ読み込み)

	ImGui::Begin("Game Draw");
	
	switch (logicType_) {
		case LogicName::LOGIN:
			ImGui::Text("Login\n");
			ImGui::InputText("Name", loginNameBuffer_, sizeof(loginNameBuffer_));
			ImGui::InputText("Password", loginPasswordBuffer_, sizeof(loginPasswordBuffer_), ImGuiInputTextFlags_Password);
			ImGui::Text("\n");

			if (ImGui::Button("Login")) {
				loginErrorText_.clear();

				loginName_ = loginNameBuffer_;
				loginPassword_ = loginPasswordBuffer_;

				// loginAsync()を呼ぶ
				string result = LoginAsync(loginName_, loginPassword_).get();

				try {
					json j = json::parse(result);

					if (j["login_status"] == "success") {
						token_ = j["token"];
						logicType_ = LogicName::WAIT_START;
					}
				}
				catch (const json::parse_error& e) {
					loginErrorText_ = "Login Error\n";
					loginErrorText_ += e.what();
				}
			}
			ImGui::Text("\n%s\n", loginErrorText_.c_str());

			if (ImGui::Button("SignUp")) {
				logicType_ = LogicName::SIGN_UP;
			}

			break;
		case LogicName::SIGN_UP:
			ImGui::Text("Sign Up\n");
			ImGui::InputText("Name", loginNameBuffer_, sizeof(loginNameBuffer_));
			ImGui::InputText("Password", loginPasswordBuffer_, sizeof(loginPasswordBuffer_), ImGuiInputTextFlags_Password);
			ImGui::Text("\n");

			if (ImGui::Button("Sign Up")) {
				loginErrorText_.clear();

				loginName_ = loginNameBuffer_;
				loginPassword_ = loginPasswordBuffer_;

				// SignUpAsync()を呼ぶ
				string result = SignUpAsync(loginName_, loginPassword_).get();

				try {
					json j = json::parse(result);

					if (j["login_status"] == "success") {
						token_ = j["token"];
						logicType_ = LogicName::WAIT_START;
					}
				}
				catch (const json::parse_error& e) {
					loginErrorText_ = "Login Error\n";
					loginErrorText_ += e.what();
				}
			}
			ImGui::Text("\n%s\n", loginErrorText_.c_str());

			if (ImGui::Button("Login")) {
				logicType_ = LogicName::LOGIN;
			}
			break;
		case LogicName::WAIT_START:
			ImGui::Text("Press SPACE to start.");
			break;
		case LogicName::RUNNING:
			ImGui::Text("STOP at 10.0sec. Press space to start.\n\n");
			if (sec_ <= 7.0f) {
				ImGui::Text("sec: %.6f", sec_);
			}
			else {
				ImGui::Text("sec: ");
			}
			break;
		case LogicName::RESULT:
			ImGui::Text("Success!! Sec: %.6f Score: %4d", sec_, score_);
			ImGui::Text("Press enter to ranking.");
			break;
		case LogicName::RANKING:
			ImGui::Text("Press space to restart.");
			break;
		default:
			break;
	}

	ImGui::End();

	ImGui::SetNextWindowPos(kDebugWindowPosRanking, ImGuiCond_Once);		// ウィンドウの座標(プログラム起動時のみ読み込み)
	ImGui::SetNextWindowSize(kDebugWindowSizeRanking, ImGuiCond_Once);		// ウィンドウのサイズ(プログラム起動時のみ読み込み)

	ImGui::Begin("Ranking Draw");

	if (logicType_ == LogicName::RANKING) {
		istringstream iss(rankingText_);
		string line;
		
		while (getline(iss, line)) {
			ImGui::Text("%s", line.c_str());
		}
	}

	ImGui::End();
}
#endif // _DEBUG

// JSONファイルの読み込み
void GameScene::LoadJsonFile([[maybe_unused]] const string& filePath) {
	// レベルデータの読み込み
	unique_ptr<JsonLevelData> levelData = jsonLoader_->LoadFile(filePath);

	// 3Dオブジェクトの読み込み
	for (const JsonObjectData& objectData : levelData->objects) {
		objectData;
	}
}

/// レスポンスを格納するためのコールバック関数
size_t GameScene::WriteCallback(void* c, size_t s, size_t n, std::string* o) {
	o->append((char*)c, s * n);
	return s * n;
}

/// ログイン処理を非同期で行う
std::future<std::string> GameScene::LoginAsync(const std::string& name, const std::string& password) {
	return std::async(std::launch::async, [name, password]() {
			CURL* curl = curl_easy_init();
			if (!curl) throw runtime_error("CURL初期化エラー");

			json body;
			body["name"] = name;
			body["password"] = password;
			std::string bodyStr = body.dump();

			std::string response;
			struct curl_slist* headers = nullptr;
			headers = curl_slist_append(headers, "Content-Type: application/json");

			curl_easy_setopt(curl, CURLOPT_URL, (string(kBaseURL) + "/users/login").c_str());
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, bodyStr.c_str());
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
			
			CURLcode res = curl_easy_perform(curl);
			curl_slist_free_all(headers);
			curl_easy_cleanup(curl);

			if (res != CURLE_OK) {
				throw runtime_error("送信エラー: " + string(curl_easy_strerror(res)));
			}

			return response;
		});
}

/// 新規登録処理
std::future<std::string> GameScene::SignUpAsync(const std::string& name, const std::string& password) {
	return std::async(std::launch::async, [name, password]() {
		CURL* curl = curl_easy_init();
		if (!curl) throw std::runtime_error("CURL 初期化エラー");

		json body;
		body["name"] = name;
		body["password"] = password;
		std::string bodyStr = body.dump();

		std::string response;
		struct curl_slist* headers = nullptr;
		headers = curl_slist_append(headers, "Content-Type: application/json");

		curl_easy_setopt(curl, CURLOPT_URL, (string(kBaseURL) + "/users/login").c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, bodyStr.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

		CURLcode res = curl_easy_perform(curl);
		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);

		if (res != CURLE_OK) {
			throw std::runtime_error("通信エラー: " + std::string(curl_easy_strerror(res)));
		}
		return response;
		});
}

/// スコア送信
std::future<std::string> GameScene::PostScoreAsync(int score, const std::string& token) {
	return std::async(std::launch::async, [score, token]() {
		CURL* curl = curl_easy_init();
		if (!curl) throw runtime_error("CURL初期化エラー");

#pragma warning(push)
#pragma warning(disable : 26495)
		json body = json::object();
		body["score"] = score;
#pragma warning(pop)

		string bodyStr = body.dump();

		struct curl_slist* headers = nullptr;
		headers = curl_slist_append(headers, "Content-Type: application/json");
		std::string auth = "Authorization: Bearer " + token;
		headers = curl_slist_append(headers, auth.c_str());

		string response;
		curl_easy_setopt(curl, CURLOPT_URL, (std::string(kBaseURL) + "/scores").c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, bodyStr.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		CURLcode res = curl_easy_perform(curl);

		// HTTPステータスコード取得
		long httpCode = 0;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);

		if (res != CURLE_OK) {
			throw runtime_error("送信エラー: " + string(curl_easy_strerror(res)));
		}

		// ステータスコード付きでレスポンスを返す
		stringstream ss;
		ss << "HTTP " << httpCode << ": " << response;
		return ss.str();
	});
}

/// スコア取得
std::future<std::string> GameScene::GetAllScoresAsync(const std::string& token) {
	return async(launch::async, [token]() {
		CURL* curl = curl_easy_init();
		if (!curl) throw runtime_error("CURL初期化エラー");

		struct curl_slist* headers = nullptr;
		std::string auth = "Authorization: Bearer " + token;
		headers = curl_slist_append(headers, auth.c_str());

		string response;
		curl_easy_setopt(curl, CURLOPT_URL, (std::string(kBaseURL) + "/scores").c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

		CURLcode res = curl_easy_perform(curl);
		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);

		if (res != CURLE_OK) {
			throw runtime_error("送信エラー: " + string(curl_easy_strerror(res)));
		}

		return response;
	});
}
