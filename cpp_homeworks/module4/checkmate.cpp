#include <iostream>
#include <array>
#include <queue>
#include <vector>

using std::vector;
using std::array;
using std::queue;
using std::string;
using std::cin;
using std::cout;

struct Figure {
    short letter;
    short number;
    Figure(short m_letter, short m_number);
};
Figure::Figure(short m_letter, short m_number) : letter(m_letter), number(m_number) {}

struct Situation {
    Figure white_queen;
    Figure black_king;
    bool move;

    Situation(short m_white_letter, short m_white_number, short m_black_letter,
              short m_black_number, bool m_move);

    Situation(const Figure& m_white_queen, const Figure& m_black_king, bool m_move);
};

Situation::Situation(const short m_white_letter,const short m_white_number,const short m_black_letter,
                     const short m_black_number,const bool m_move) :
        white_queen(Figure(m_white_letter, m_white_number)),
        black_king(Figure(m_black_letter, m_black_number)),
        move(m_move) {}
Situation::Situation(const Figure& m_white_queen,const Figure& m_black_king,const bool m_move) :
        white_queen(m_white_queen), black_king(m_black_king), move(m_move) {}

class CheckMate {
public:
    CheckMate();
    void Solve(short queen_letter,short queen_number, short king_letter, short king_number);
private:
    const short TABLE_SIZE = 8;
    array<array<array<array<array<short, 2>, TABLE_SIZE>, TABLE_SIZE>, TABLE_SIZE>, TABLE_SIZE> positions;
    queue<Situation> q;

    short get_pos(const Situation& tmp);

    vector<Figure> king_step(const Situation& tmp) ;
    void black_move(const Situation& tmp);

    vector<Figure> white_step(const Situation& tmp);
    int white_move(const Situation& tmp);
};

CheckMate::CheckMate() {
    for (short white_letter = 0; white_letter < TABLE_SIZE; ++white_letter) {
        for (short white_num = 0; white_num < TABLE_SIZE; ++white_num) {
            for (short black_letter = 0; black_letter < TABLE_SIZE; ++black_letter) {
                for (short black_num = 0; black_num < TABLE_SIZE; ++black_num) {
                    for(short type = 0; type < 2; ++type) {
                        positions[white_letter][white_num][black_letter][black_num][type] = -1;

                        if (black_letter >= 1 && black_letter <= 3 && black_num >= 1 && black_num <= 3) {
                            positions[white_letter][white_num][black_letter][black_num][type] = 100;
                        }

                        if (white_letter == 2 && white_num == 2) {
                            positions[white_letter][white_num][black_letter][black_num][type] = 100;
                        }

                        if ((white_letter == black_letter || white_num == black_num ||
                             white_letter + white_num == black_letter + black_num ||
                             white_letter - white_num == black_letter - black_num)) {
                            positions[white_letter][white_num][black_letter][black_num][0] = 100;
                        }

                        if ((type == 1) && (white_letter - black_letter <= 1) && (white_num - black_num <= 1)
                            && (black_letter - white_letter <= 1) && (black_num - white_num <= 1)) {
                            if (white_letter == 1 || white_letter == 2 || white_letter == 3) {
                                if (white_num == 1 || white_num == 2 || white_num == 3) {
                                    if (white_letter != 2 && white_num != 2) {
                                        break;
                                    }
                                }
                            }
                            positions[white_letter][white_num][black_letter][black_num][1] = 100;
                        }
                    }
                }
            }
        }
    }
}
void CheckMate::Solve(const short queen_letter,const short queen_number,const short king_letter,const short king_number) {
    // Массив, чтобы задать конечные положения через цикл. Иначе получается много однотипности.
    // В цикле формула.
    array<short, 19> endPos = {450, 707, 323, 16, 1104,
                               2064, 642, 258, 322, 584,
                               2576, 3088, 576,  3600, 1624,
                               2584, 577, 2,  386};

    for(short pos : endPos) {
        short white_pos = pos / TABLE_SIZE*TABLE_SIZE;
        short white_letter = white_pos / TABLE_SIZE;
        short white_num = white_pos % TABLE_SIZE;

        short black_pos = pos % TABLE_SIZE*TABLE_SIZE;
        short black_letter = black_pos / TABLE_SIZE;
        short black_num = black_pos % TABLE_SIZE;

        positions[white_letter][white_num][black_letter][black_num][1] = 0;
        q.push(Situation(white_letter,white_num,black_letter,black_num, false));
    }

    positions[queen_letter][queen_number][king_letter][king_number][0] = -2;

    while(!q.empty()) {
        Situation tmp = q.front(); q.pop();
        if(tmp.move == false) {
            int step = white_move(tmp);

            if(step > 0) {
                cout << step;
                return;
            }
        }
        else {
            black_move(tmp);
        }
    }
}

short CheckMate::get_pos(const Situation& tmp) {
    short queen_letter = tmp.white_queen.letter;
    short queen_number = tmp.white_queen.number;
    short king_letter = tmp.black_king.letter;
    short king_number = tmp.black_king.number;

    return positions[queen_letter][queen_number][king_letter][king_number][tmp.move];
}

vector<Figure> CheckMate:: king_step(const Situation& tmp) {
    vector<Figure> ans;
    short letter = tmp.black_king.letter;
    short number = tmp.black_king.number;
    if (letter > 0) {
        ans.push_back(Figure(letter-1, number));
    }
    if (letter < TABLE_SIZE-1) {
        ans.push_back(Figure(letter+1, number));
    }
    if (number > 0) {
        ans.push_back(Figure(letter, number-1));
    }
    if (number < TABLE_SIZE-1) {
        ans.push_back(Figure(letter, number+1));
    }
    if (letter > 0 && number > 0) {
        ans.push_back(Figure(letter-1, number-1));
    }
    if (letter > 0 && number < TABLE_SIZE-1) {
        ans.push_back(Figure(letter-1, number+1));
    }
    if (letter < TABLE_SIZE-1 && number > 0) {
        ans.push_back(Figure(letter+1, number-1));
    }
    if (letter < TABLE_SIZE-1 && number < TABLE_SIZE-1) {
        ans.push_back(Figure(letter+1, number+1));
    }
    return ans;
}
void CheckMate::black_move(const Situation& tmp) {
    vector<Figure> prev_king_pos = king_step(tmp);

    for(Figure next_pos : prev_king_pos) {
        if(get_pos(Situation(tmp.white_queen,next_pos, true)) < 0) {
            bool flag = true;
            vector<Figure> something = king_step(Situation(tmp.white_queen, next_pos, true));
            for(Figure last:something) {
                if(get_pos(Situation(tmp.white_queen,last, false)) < 0) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                positions[tmp.white_queen.letter][tmp.white_queen.number][next_pos.letter][next_pos.number][1] =
                        get_pos(Situation(tmp.white_queen, tmp.black_king, 0))  + 1;
                q.push(Situation(tmp.white_queen, next_pos, false));
            }
        }
    }
}

vector<Figure>CheckMate:: white_step(const Situation& tmp) {
    vector<Figure> ans;
    Figure white_queen = tmp.white_queen;
    short queen_let = white_queen.letter;
    short queen_num = white_queen.number;

    for(short up = 0; up < TABLE_SIZE; up++){
        for(short side = 0; side < TABLE_SIZE; side++) {
            short queen_previous_letter = (queen_let+up)%TABLE_SIZE;
            short queen_previous_number = (queen_num+side)%TABLE_SIZE;
            if(get_pos(Situation(Figure(queen_previous_letter,queen_previous_number), tmp.black_king, 0)) < 0) {
                if ((queen_let == queen_previous_letter || queen_num == queen_previous_number ||
                     queen_let + queen_num == queen_previous_letter + queen_previous_number ||
                     queen_let - queen_num == queen_previous_letter - queen_previous_number) &&
                    ((queen_let != queen_previous_letter) || (queen_num != queen_previous_number))) {
                    if (((queen_let - queen_previous_letter) * (queen_num - 2) != (queen_num -
                                                                                   queen_previous_number) * (queen_let - 2) ||
                         (queen_let - 2) * (queen_previous_letter - 2) > 0
                         || (queen_num - 2) * (queen_previous_number - 2) > 0)) {
                        ans.push_back(Figure(queen_previous_letter, queen_previous_number));
                    }
                }
            }
        }
    }
    return ans;
}
int CheckMate::white_move(const Situation& tmp) {
    vector<Figure> new_pos = white_step(tmp);

    for (Figure white:new_pos) {
        if (get_pos(Situation(white,tmp.black_king, false)) == -2) {
            return (get_pos(Situation(tmp.white_queen, tmp.black_king, true)) + 1);
        } else {
            positions[white.letter][white.number][tmp.black_king.letter][tmp.black_king.number][0]
                    = get_pos(Situation(tmp.white_queen, tmp.black_king, true))  + 1;
            q.push(Situation(white,tmp.black_king, true));
        }
    }
    return -1;
}

int main()
{
    string first;
    string second;

    cin >> first;
    cin >> second;

    short queen_letter = first[0] - 'a';
    short queen_number = short(first[1]) - '0' - 1;
    short king_letter = second[0] - 'a';
    short king_number = short(second[1]) - '0' - 1;

    CheckMate Solver;
    Solver.Solve(queen_letter, queen_number, king_letter, king_number);
    return 0;
}
