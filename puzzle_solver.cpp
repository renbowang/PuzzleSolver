#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <algorithm>

using namespace std;
using Block = vector<string>;

Block board;

struct Piece {
	vector<pair<pair<int, int>,int>>placeble_pos_and_datas_idx;
	vector<Block>datas;//4 spin variatians
	bool symmetry = false;//if true=>no need to spin
	bool symmetry_long = false;//if true=>only need to spin twice
	int non_zero_element_counts=0;
	int current_idx = 0;
	char eigenvalue;

	void init() {
		trim();
		Block original_data = datas[0];
		init_non_zero();
		spin();
		if (datas[0] == original_data)
			symmetry = true;
		else
			datas.push_back(datas[0]);
		spin();
		if (symmetry||(datas[0] == original_data)) 
			symmetry_long = true;
		else
			datas.push_back(datas[0]);
		if (!symmetry&&!symmetry_long) {
			spin();
			datas.push_back(datas[0]);//spin thrice
		}
		datas[0] = original_data;
		find_all_pieces_placeble_pos();
	}

	void init_non_zero() {
		for (int i = 0; i < datas[0].size(); ++i)
			for (int j = 0; j < datas[0][0].size(); ++j)
				if (datas[0][i][j] != '0') {
					non_zero_element_counts++;
					eigenvalue = datas[0][i][j];
				}
	}

	void spin() {
		if (datas[0].empty() || datas[0][0].empty()) return;
		int r = datas[0].size(),c= datas[0][0].size();
		Block rotated_piece(c, string(r, ' '));

		for (int i = 0; i < r; i++) {
			for (int j = 0; j < c; j++) {
				rotated_piece[j][r - 1 - i] = datas[0][i][j];
			}
		}
		datas[0] = rotated_piece;
	}

	void trim() {
		if (datas[0].empty() || datas[0].empty()) return;

		int minR = datas[0].size(), maxR = -1;
		int minC = datas[0].size(), maxC = -1;

		for (int r = 0; r < datas[0].size(); ++r)
			for (int c = 0; c < datas[0][0].size(); ++c)
				if (datas[0][r][c] != '0') {
					minR = min(minR, r);
					maxR = max(maxR, r);
					minC = min(minC, c);
					maxC = max(maxC, c);
				}

		if (maxR == -1) {
			datas[0].clear();
			return;
		}

		Block trmmed_data;
		for (int r = minR; r <= maxR; ++r) {
			trmmed_data.push_back(datas[0][r].substr(minC, maxC - minC + 1));
		}
		datas[0] = trmmed_data;
	}

	void find_all_pieces_placeble_pos() {
		for (int k = 0; k < datas.size(); ++k)
			for (int r = 0; r <= board.size() - datas[k].size(); ++r)
				for (int c = 0; c <= board[0].size() - datas[k][0].size(); ++c) {
					int same_counts = 0;

					for (int bi = 0; bi < datas[k].size(); ++bi)
						for (int bj = 0; bj < datas[k][0].size(); ++bj) {
							if (datas[k][bi][bj] != '0') {
								int board_r = r + bi;
								int board_c = c + bj;
								if (board_r >= board.size() || board_c >= board[0].size() || board[board_r][board_c] != 'Z')
									break;
								same_counts++;
								if (same_counts == non_zero_element_counts)
									placeble_pos_and_datas_idx.push_back(make_pair(make_pair(r, c), k));
							}
						}
				}
	}
};


vector<Piece> pieces;



bool read_input_file(string filename) {
	ifstream f(filename);
	if (!f.is_open()) {
		cerr << "could not open " << filename << '\n';
		return false;
	}
	
	string getline_str;

	//read board
	while (getline(f, getline_str)) {
		string board_str;
		if (getline_str == "x")
			break;
		for (char c : getline_str)
			if (c != ' ')
				if (c == '1')
					board_str += 'Z';
				else
					board_str += '0';
		if (!board_str.empty())
			board.push_back(board_str);
	}
	//read board end
	//read pieces
	Block current_piece;
	while (getline(f, getline_str)) {
		string piece_str;
		for (char c : getline_str)
			if (c != ' ')
				piece_str += c;
		if (piece_str == "0")
			break;
		if (piece_str == "x") {
			if (!current_piece.empty()) {
				Piece piece;
				piece.datas.push_back(current_piece);
				piece.init();
				pieces.push_back(piece);
				current_piece.clear();
			}
			continue;
		}
		if (!piece_str.empty())
			current_piece.push_back(piece_str);
	}
	//last block
	if (!current_piece.empty()) {
		Piece piece;
		piece.datas.push_back(current_piece);
		piece.init();
		pieces.push_back(piece);
	}

	return true;
	f.close();
}

void sort_by_pos_size() {
	sort(pieces.begin(), pieces.end(), [](const Piece& a, const Piece& b) {
		return a.placeble_pos_and_datas_idx.size() < b.placeble_pos_and_datas_idx.size();});
}
bool puzzle_solver(int k) {
	if (k == pieces.size())
		return true;
	//pruning: calculate remaining unplaced cells on the board
	int remaining_empty_cells = 0;
	for (int r = 0; r < board.size(); ++r)
		for (int c = 0; c < board[0].size(); ++c)
			if (board[r][c] == 'Z')
				remaining_empty_cells++;
	//pruning: calculate total non zero cells for remaining blocks
	int remaining_blocks_non_zero = 0;
	for (int i = k; i < pieces.size(); ++i)
		remaining_blocks_non_zero += pieces[i].non_zero_element_counts;
	//prune
	if (remaining_empty_cells < remaining_blocks_non_zero)
		return false;

	for (int i = pieces[k].current_idx; i < pieces[k].placeble_pos_and_datas_idx.size(); ++i) {
		pieces[k].current_idx = i;

		pair<pair<int, int>, int>current_placement = pieces[k].placeble_pos_and_datas_idx[i];
		int r = current_placement.first.first;
		int c = current_placement.first.second;
		int spin_count = current_placement.second;

		Block current_block_data = pieces[k].datas[spin_count];
		vector<pair<int, int>>placed_board_idx;
		bool can_place = true;
		for (int bi = 0; can_place&&bi < current_block_data.size(); ++bi)
			for (int bj = 0; can_place && bj < current_block_data[0].size(); ++bj)
				if (current_block_data[bi][bj] != '0') {
					int board_r = r + bi;
					int board_c = c + bj;

					// Check bounds and if the puzzle cell is empty ('Z')
					if (board_r >= board.size() || board_c >= board[0].size() ||
						board[board_r][board_c] != 'Z') {
						can_place = false;
						break;
					}


					placed_board_idx.push_back({ board_r, board_c });
				}

		if (can_place) {
			for (const auto& cell : placed_board_idx)
				board[cell.first][cell.second] = pieces[k].eigenvalue;
			if (puzzle_solver(k + 1))
				return true;
			//if k+1 false=>backtrack
			for (const auto& cell : placed_board_idx)
				board[cell.first][cell.second] = 'Z';
		}
	}
	pieces[k].current_idx = 0;
	return false;
}

void print() {
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[0].size(); j++) {
			// Original code used 'Z' for '1's, converting back for output
			if (board[i][j] == 'Z') {
				cout << "1 ";
			}
			else {
				cout << board[i][j] << " ";
			}
		}
		cout << endl;
	}
}

int main(int argc,char*argv[]) {
	if (read_input_file(argv[1])) {
		sort_by_pos_size();
		if (puzzle_solver(0))
			print();
		else
    		cout << "no solution";
	}
}