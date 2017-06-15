

template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<string> split(const string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, back_inserter(elems));
	return elems;
}

float calcWER(string reference,string hypothesis){

	float WER = 0;

//	string reference = "";
	//string hypothesis = "who is there";

	vector<string> ref = split(reference, ' ');
	vector<string> hyp = split(hypothesis, ' ');



	//rezerwacja pamieci na macierz D
	int** D = new int*[ref.size() + 1];
	for (int i = 0; i < ref.size() + 1; i++){
		D[i] = new int[hyp.size() + 1];
	}

	for (int i = 0; i < ref.size() + 1; i++){
		for (int j = 0; j < hyp.size() + 1; j++){
			if (i == 0) {
				D[0][j] = j;

			}
			else if (j == 0){
				D[i][0] = i;
			}

		}
	}

	float substitution, insertion, deletion;

	for (int i = 1; i < ref.size() + 1; i++){
		for (int j = 1; j < hyp.size() + 1; j++){
			if (ref[i - 1] == hyp[j - 1]){
				D[i][j] = D[i - 1][j - 1];
			}
			else{
				substitution = D[i - 1][j - 1] + 1;
				insertion = D[i][j - 1] + 1;
				deletion = D[i - 1][j] + 1;
				D[i][j] = min({ substitution, insertion, deletion });
			}
		}
	}

	std::cout << D[ref.size()][hyp.size()];

	if (ref.size() > hyp.size()) WER = D[ref.size()][hyp.size()] / ref.size();
	else WER = D[ref.size()][hyp.size()] / hyp.size();


	//zwolnienie pamieci macierzy D
	for (int i = 0; i < ref.size() + 1; i++){
		delete[] D[i];
	}
	delete[] D;

	return WER;
}