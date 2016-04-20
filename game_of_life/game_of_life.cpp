#include <array>
#include <string>
#include <thread>
#include <chrono>
#include <utility>
#include <iostream>

using namespace std;

/*
Any live cell with fewer than two live neighbours dies, as if caused by under-population.
Any live cell with two or three live neighbours lives on to the next generation.
Any live cell with more than three live neighbours dies, as if by over-population.
Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/
#define size  20

template<size_t I, size_t J,class T>
decltype(auto) print_board(array<array<T, I>, J>& board) {
	for (auto i : board) {
		for (auto j : i) {
			cout << (j?"#":".") ;
		}
		cout << endl;
	}

	cout << string( J, '+' )<<endl;
}


template<size_t I,size_t J>
decltype(auto) compute_round(array<array<bool, I>, J>& board) {
	auto new_board(board);
	for (auto i = 1; i < I - 1; i++) {
		for (auto j = 1; j < J - 1; j++) {
			auto local_population =
			 	board[i + 1][j + 1] +
		 		board[i + 1][j - 1] +
			 	board[i - 1][j + 1] +
		 		board[i - 1][j - 1] +
				board[i + 1][j    ] +
				board[i - 1][j    ] +
				board[i    ][j + 1] +
				board[i    ][j - 1];
			switch (local_population){
			case 2:
				break;
			case 3:
				new_board[i][j] = true;
				break;
			default:
				new_board[i][j] = false;
			}
		}
	}
	board = new_board;
}

array<array<bool, 20 + 2>, 20 + 2>  test = { array<bool, 20 + 2>
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

template<size_t round, size_t x, size_t y>
struct cell {
	enum {
		local_population=
		(x + 1 != size + 2 ? (y + 1 != size + 2 ? cell<round - 1, x + 1, y + 1>::val : 0) : 0) +
		(x + 1 != size + 2 ? (y - 1 != 0 ?        cell<round - 1, x + 1, y - 1>::val : 0) : 0) +
		(x - 1 != 0 ? (y + 1 != size + 2 ?		  cell<round - 1, x - 1, y + 1>::val : 0) : 0) +
		(x - 1 != 0 ? (y - 1 != 0 ?				  cell<round - 1, x - 1, y - 1>::val : 0) : 0) +
		(x + 1 != size + 2 ? (y != 0 ?			  cell<round - 1, x + 1, y    >::val : 0) : 0) +
		(x - 1 != 0 ? (y != 0 ?					  cell<round - 1, x - 1, y    >::val : 0) : 0) +
		(x != 0 ? (y + 1 != size + 2 ?			  cell<round - 1, x    , y + 1>::val : 0) : 0) +
		(x != 0 ? (y - 1 != 0 ?					  cell<round - 1, x    , y - 1>::val : 0) : 0) 
	};
	enum {
		val = (cell<round, x, y>::local_population == 2 ? cell<round - 1, x, y>::val :
			(cell<round, x, y>::local_population == 3 ? 1 : 0)
		)
	};
};

template<size_t x, size_t y>
struct cell<0, x, y> {
	enum { val=0 };
};

/*
##
##
*/
template<>struct cell<0, 1, 1> { enum { val = 1 }; };
template<>struct cell<0, 1, 2> { enum { val = 1 }; };
template<>struct cell<0, 2, 1> { enum { val = 1 }; };
template<>struct cell<0, 2, 2> { enum { val = 1 }; };

/*
 ##
#  #
 ##
*/
template<>struct cell<0, 5, 5> { enum { val = 1 }; };
template<>struct cell<0, 6, 5> { enum { val = 1 }; };
template<>struct cell<0, 4, 6> { enum { val = 1 }; };
template<>struct cell<0, 7, 6> { enum { val = 1 }; };
template<>struct cell<0, 5, 7> { enum { val = 1 }; };
template<>struct cell<0, 6, 7> { enum { val = 1 }; };

/*
###
*/
template<>struct cell<0, 10, 10> { enum { val = 1 }; };
template<>struct cell<0, 11, 10> { enum { val = 1 }; };
template<>struct cell<0, 12, 10> { enum { val = 1 }; };

/*
 #
  #
###
*/
template<>struct cell<0, 15, 12> { enum { val = 1 }; };
template<>struct cell<0, 16, 10> { enum { val = 1 }; };
template<>struct cell<0, 16, 12> { enum { val = 1 }; };
template<>struct cell<0, 17, 11> { enum { val = 1 }; };
template<>struct cell<0, 17, 12> { enum { val = 1 }; };


template<size_t round, size_t x, size_t... y> constexpr
decltype(auto) meta_row(index_sequence< y...>) {
	return array<size_t, sizeof...(y)>{ 
		{ cell<round, y, x>::val... }
	};
}

template<size_t round, size_t y_size, size_t... x> constexpr
decltype(auto) meta_table(index_sequence< x...>) {
	return array<decltype(meta_row<round, sizeof...(x)>(make_index_sequence<y_size>())),y_size>
	{
		{ meta_row<round,x>(make_index_sequence<y_size>())... }
	};
}

#define life_board(round,x_size,y_size) meta_table<round,y_size>(make_index_sequence<x_size>())

template<size_t x_dim, size_t y_dim, size_t... round> constexpr
decltype(auto) _life_game(index_sequence<round...>) {
	return array<decltype(life_board(0, x_dim, y_dim)), sizeof...(round)>{
		{life_board(round, x_dim, y_dim)...}
	};
}

//#define life_game(x_dim,y_dim,rounds) _life_game<x_dim,y_dim>(make_index_sequence<rounds>())
//assume board is square
#define life_game(x_dim,rounds) _life_game<x_dim,x_dim>(make_index_sequence<rounds>())

int main(int argc,char** argv) {

	for (auto i : life_game(size+2 , 12)) {
		print_board(i);
		this_thread::sleep_for(1s);		
	}
	/*
	array<array<bool, size + 2>, size + 2> board= test;

	for (auto i = 0; i < 20; i++) {
		print_board(board);
		compute_round(board);
		system("pause");
	}
	cout << "finished" << endl;
	system("pause");
	*/

	return 0;

}