/**
 * EX5 - Created by Bar Melinarskiy - 318189982 on 6/29/2020
 */
#ifndef EX5PROJECT_RECOMMENDERSYSTEM_H
#define EX5PROJECT_RECOMMENDERSYSTEM_H

#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <iomanip>
#include <iterator>
#include <math.h>
#include <functional>
#include <numeric>
#include <limits>

using namespace std;

#define ZERO 0.0
#define FAILURE -1
#define SUCCESS 0
#define NEW_LINE "\n"
#define NO_RATING "NA"

const double INIT_VALUE = 0.0;
/**
 * @class Recommender System Class
 * @property _numberOfMovies - # of movies in _moviesMap
 * @property _moviesMap - map of the movies' related info
 * @property _movies - vector of the movies to be rated by people
 * @property _numberOfPeople - # of people in _ratings
 * @property _ratings - map of the people ratings
 */
class RecommenderSystem
{
	public:
		/**
		* Default Constructor
		* Constructs an empty maps and sets the counters to 0
		*/
		RecommenderSystem();
		/**
		* Fills the class' maps from the given files
		* @param moviesFilePath - the movies attributes file path
		* @param ranksFilePath - the user ranks file path)
		* @return 0 on success , -1 otherwise
		*/
		int loadData(const string moviesFilePath, const string ranksFilePath);
		/**
		* Gets the recommended movie for the given user, calc by movie's content
		* @param person - the person to recommend to
		* @return the recommended movie
		*/
		string recommendByContent(const string person) const;
		/**
		* predict the user's score for the given user
		* @param movieName - the movie to calc it's score
		* @param userName - the person to recommend to
		* @param k - positive int, the number of movies to take into account
		* @return the recommended movie
		*/
		double predictMovieScoreForUser(const string movieName, const string userName,
										const unsigned int k) const;
		/**
		* Gets the recommended movie for the given user, calc by CF
		* @param userName - the person to recommend to
		* @param k - positive int, the number of movies to take into account
		* @return the recommended movie
		*/
		string recommendByCF(const string& userName, const unsigned int k) const;
	private:
		//type defs
		typedef vector<double> VectorProperty;
		typedef map <const string, VectorProperty> MoviesMap;
		typedef vector<pair<bool, double>> VectorRatings;
		typedef map <const string, VectorRatings> RatingsMap;
		typedef vector<string> MovieVector;
		typedef pair<string, double> RatingPair;
		typedef vector<RatingPair> MovieRating;
		/**
		*
		* @struct Struc Cmp for set comp function
		*/
		struct Cmp
		{
			/**
			* Comp function for set
			* @param a - first element
			* @param b - second element
			* @return true if second element is bigger, false otherwise
			*/
			bool operator ()(const RatingPair &a, const RatingPair &b) const
			{
				return a.second < b.second;
			}
		};
		typedef set<RatingPair, Cmp> MovieSet;
		typedef RatingsMap::const_iterator iteratorRating;
		typedef MoviesMap::const_iterator iteratorMovie;

		/**
		* Get movie info by movie name
		* @param movieName - movie to fetch
		* @return the movie's info from _moviesMap
		*/
		auto _getMovieInfo(const string movieName) const;
		/**
		* Get movie info by movie index in _movies
		* @param movieName - movie to fetch
		* @return the movie's info from _moviesMap
		*/
		auto _getMovieInfoByIndex(const int index) const;

		/**
		* Get movie info by movie index in _movies
		* @param movieName - movie to fetch it's rating
		* @return the movie's rating
		*/
		double _getMovieRating(const string movieName, const iteratorRating &itPerson) const;

		/**
		* Fills the _movies vector
		* @param iss - data to insert
		*/
		void _setMoviesArray(istringstream &iss);
		/**
		* Fills the ratings values vector
		* @param values - vector to change
		* @param iss - data to insert
		*/
		void _setVectorRatings(VectorRatings &values, istringstream &iss);

		/**
		* Fills the _moviesMap map from the given file
		* @param moviesFilePath - the movies attributes file path
		* @return 0 on success , -1 otherwise
		*/
		int _loadMovies(const string moviesFilePath);

		/**
		* Fills the _ratings map from the given file
		* @param ranksFilePath - the user ranks file path)
		* @return 0 on success , -1 otherwise
		*/
		int _loadRatings(const string moviesFilePath);

		/**
		* Overload * for vector with scalar
		* @param v - vector to multiple
		* @param c - the scalar
		* @return the result vector
		*/
		friend RecommenderSystem::VectorProperty operator * (const VectorProperty& v,
															 const double c);
		/**
		* Overload + for vectors
		* @param v1 - first vector to add too
		* @param v1 - the second vector to add from
		* @return the sum vector
		*/
		friend RecommenderSystem::VectorProperty operator + (const VectorProperty& v1,
															 const VectorProperty& v2);
		/**
		* Overload * for vectors
		* @param v1 - first vector
		* @param v1 - the second vector
		* @return the * result
		*/
		friend double operator * (const VectorProperty& v1, const VectorProperty& v2);
		/**
		* Calc the norm of the given vector
		* @param v - given vector
		* @return the vector's norm
		*/
		double _calcVectorNorm(const VectorProperty& vector) const;

		/**
		* Calc the angle btw two given vectors
		* @param v1 - first vector
		* @param v2 - second vector
		* @return the angle
		*/
		double _calcAngleBtw(const VectorProperty& v1, const VectorProperty& v2) const;

		/**
		* Calc unit vector
		* @param vectorUnit - vector to change
		* @param preferencesVector - the person's preferences vector
		* @return 0 on success , -1 otherwise
		*/
		int _calcUnitVector(VectorRatings &unitVector, VectorProperty &preferencesVector) const;

		/**
		* Calc movie to recommend according to the preferences Vector
		* @param vectorUnit - norm of rating vector
		* @param preferencesVector -  the preferences Vector
		* @return the vector's norm
		*/
		string _calcRecommendMovieByContent(const VectorRatings &unitVector,
											const VectorProperty& preferencesVector) const;
		/**
		* Get the k most similar movies
		* @param movieVector - the angle vector of movies
		* @param k - positive int, the number of movies to take into account
		* @return the vector of the k most similar movies
		*/
		MovieSet _getKMaxMovies(const MovieRating &movieVector, const unsigned int k) const;
		/**
		* Get the movie score according to the CF formula
		* @param itMovie - the movie's info
		* @param itPerson - the person's ratings
		* @param k - positive int, the number of movies to take into account
		* @return the movie prediction score
		*/
		double _getMovieScore(const iteratorMovie &itMovie, const iteratorRating &itPerson,
							  const unsigned int k) const;
		/**
		* Calc the movie score according to the CF formula
		* @param movieSet - set of the the k most similar movies
		* @param itPerson - the person's ratings
		* @return the movie prediction score
		*/
		double _calcMovieScore(const MovieSet &movieSet, const iteratorRating &itPerson) const;

		int _numberOfMovies;
		MoviesMap _moviesMap;
		MovieVector _movies;
		int _numberOfPeople;
		RatingsMap _ratings;
};


#endif //EX5PROJECT_RECOMMENDERSYSTEM_H
