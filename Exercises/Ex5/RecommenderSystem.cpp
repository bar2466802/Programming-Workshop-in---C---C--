//
// Created by BarM on 6/29/2020.
//

#include "RecommenderSystem.h"


#define ERROR_OPEN_FILE "Unable to open file "
#define ERROR_NO_USER "USER NOT FOUND"
#define NO_RATINGS "NO RATINGS"
#define ERROR_NO_MOVIE "MOVIE NOT FOUND"
/**
* Default Constructor
* Constructs an empty maps and sets the counters to 0
*/
RecommenderSystem::RecommenderSystem(): _numberOfMovies(ZERO), _numberOfPeople(ZERO)
{
	//Create maps with Default constructor
	MoviesMap _moviesMap;
	MovieVector _movieVector;
	RatingsMap _ratings;
}
/**
* Fills the class maps from the given files
* @param moviesFilePath - the movies attributes file path
* @param ranksFilePath - the user ranks file path)
* @return 0 on success , -1 otherwise
*/
int RecommenderSystem::loadData(const string moviesFilePath, const string ranksFilePath)
{
	if(_loadMovies(moviesFilePath) == FAILURE)
	{
		return FAILURE;
	}
	return _loadRatings(ranksFilePath);
}
/**
* Fills the _moviesMap map from the given file
* @param moviesFilePath - the movies attributes file path
* @return 0 on success , -1 otherwise
*/
int RecommenderSystem::_loadMovies(const string moviesFilePath)
{
	ifstream file(moviesFilePath);
	if (file.is_open()) //try to open the file
	{
		string line;
		while (getline(file, line) && line != NEW_LINE) //loop until we reach new-line
		{
			istringstream iss(line);
			int index = 0;
			VectorProperty values;
			string movieName = "";
			//Split the string on space
			for(string s; iss >> s; )
			{
				if(index == 0) //get the movie's name
				{
					movieName = s;
				}
				else
				{
					values.push_back(atof(s.c_str())); //insert the property value to the vector
				}
				index++;
			}
			values.shrink_to_fit();
			_moviesMap[movieName] = values; //insert new element to the movies' map
			_numberOfMovies++; //update movie counter
		}
	}
	else
	{
		// Handling for file not able to be opened
		cerr << ERROR_OPEN_FILE << moviesFilePath << endl;
		return FAILURE;
	}
	file.close();
	return SUCCESS;
}
/**
* Fills the _ratings map from the given file
* @param ranksFilePath - the user ranks file path)
* @return 0 on success , -1 otherwise
*/
int RecommenderSystem::_loadRatings(const string ranksFilePath)
{
	ifstream file(ranksFilePath);
	if (file.is_open()) //try to open the file
	{
		string line;
		while (getline(file, line) && line != NEW_LINE) //loop until we reach new-line
		{
			istringstream iss(line);
			if(_movies.empty()) //Check is this is the first line with the movies' names
			{
				//fill the movies' names
				_setMoviesArray(iss);
			}
			else
			{
//				int index = 0;
				VectorRatings values;
				_setVectorRatings(values, iss);
//				string personName = "";
//				//Split the string on space
//				for(string s; iss >> s; )
//				{
//					if(index == 0) //get the person's name
//					{
//						personName = s;
//					}
//					else
//					{
//						if(s != NO_RATING)
//						{
//							//insert the property value to the vector
//							values.push_back(make_pair(true, atof(s.c_str())));
//						}
//						else
//						{
//							//insert the property value to the vector
//							values.push_back(make_pair(false, INIT_VALUE));
//						}
//					}
//					index++;
//				}
//				values.shrink_to_fit();
//				_ratings[personName] = values; //insert new element to the ratings' map
//				_numberOfPeople++; //update movie counter
			}
		}
	}
	else
	{
		// Handling for file not able to be opened
		cerr << ERROR_OPEN_FILE << ranksFilePath << endl;
		return FAILURE;
	}
	file.close();
	return SUCCESS;
}

auto RecommenderSystem::_getMovieInfo(const string movieName) const
{
	auto itMovie = _moviesMap.find(movieName);
	return itMovie;
}

auto RecommenderSystem::_getMovieInfoByIndex(const int index) const
{
	string movie = _movies[index];
	return _getMovieInfo(movie);
}

/**
* Gets the recommended movie for the given user, calc by movie's content
* @param person - the person to recommend to
* @return the recommended movie
*/
string RecommenderSystem::recommendByContent(const string person) const
{
	auto itPerson = _ratings.find(person);
	//Check the person exist
	if(itPerson == _ratings.end())
	{
		return ERROR_NO_USER;
	}
	VectorRatings unitVector = itPerson->second; //get the person's ratings info
	VectorProperty preferencesVector (unitVector.size(), INIT_VALUE); //get the person's ratings
	// info
	if(_calcUnitVector(unitVector, preferencesVector) == FAILURE)
	{
		return NO_RATINGS;
	}

	return _calcRecommendMovieByContent(unitVector, preferencesVector);
}
/**
* Overload * for vector with scalar
* @param v - vector to multiple
* @param c - the scalar
* @return the result vector
*/
RecommenderSystem::VectorProperty operator*(const RecommenderSystem::VectorProperty &v,
											const double c)
{
	//vector * scalar
	RecommenderSystem::VectorProperty result = v;
	for(auto& itProperty: result)
	{
		itProperty *= c;
	}
	return result;
}
/**
* Overload + for vectors
* @param v1 - first vector to add too
* @param v1 - the second vector to add from
* @return the sum vector
*/
RecommenderSystem::VectorProperty operator+(const RecommenderSystem::VectorProperty &v1,
											const RecommenderSystem::VectorProperty &v2)
{
	RecommenderSystem::VectorProperty result = v1;
	int index = 0;
	for(auto& itProperty: result)
	{
		itProperty += v2[index];
		index++;
	}
	return result;
}
/**
* Overload * for vectors
* @param v1 - first vector
* @param v1 - the second vector
* @return the * result
*/
double operator*(const RecommenderSystem::VectorProperty &v1,
				 const RecommenderSystem::VectorProperty &v2)
{
	double product = inner_product(v1.begin(), v1.end(), v2.begin(), INIT_VALUE);
	return product;
}
/**
* Calc unit vector
* @param vectorUnit - vector to change
* @param preferencesVector - the person's preferences vector
* @return 0 on success , -1 otherwise
*/
int RecommenderSystem::_calcUnitVector(RecommenderSystem::VectorRatings &unitVector,
									   RecommenderSystem::VectorProperty &preferencesVector) const
{
	double sum = INIT_VALUE;
	int counter = 0;
	for (auto v : unitVector)
	{
		if (v.first == true) //check the current has ratted by this person
		{
			sum += v.second;
			counter++;
		}
	}

	if (counter == 0)
	{
		return FAILURE;
	}
	double avg = sum / (double) counter;
	int index = 0;
	for (auto v : unitVector)
	{
		if (v.first == true) //check the current has ratted by this person
		{
			//phase 1 in the method
			v.second -= avg;
			string movie = _movies[index];
			auto itMovie = _moviesMap.find(movie);
			//Check the person exist
			if (itMovie == _moviesMap.end())
			{
				return FAILURE; //Movie not found
			}
			else
			{
				//phase 2 in the method
				VectorProperty moviesProperty = itMovie->second;
				preferencesVector = preferencesVector + (moviesProperty * v.second);
			}
		}
		index++;
	}
	return SUCCESS;
}
/**
* Calc the norm of the given vector
* @param v - given vector
* @return the vector's norm
*/
double RecommenderSystem::_calcVectorNorm(const RecommenderSystem::VectorProperty &vector) const
{
	double sum = 0.0;
	for (auto value : vector)
	{
		sum += value * value;
	}
	return sqrt(sum);
}
/**
* Calc the angle btw two given vectors
* @param v1 - first vector
* @param v2 - second vector
* @return the angle
*/
double RecommenderSystem::_calcAngleBtw(const RecommenderSystem::VectorProperty &v1,
										const RecommenderSystem::VectorProperty &v2) const
{
	double denominator = _calcVectorNorm(v1) * _calcVectorNorm(v2);
	double numerator = v1 * v2;
	if(denominator != ZERO)
	{
		return numerator / denominator;
	}
	else
	{
		return ZERO;
	}
}
/**
* Calc movie to recommend according to the preferences Vector
* @param vectorUnit - norm of rating vector
* @param preferencesVector -  the preferences Vector
* @return the vector's norm
*/
string RecommenderSystem::_calcRecommendMovieByContent(
		const RecommenderSystem::VectorRatings &unitVector,
		const RecommenderSystem::VectorProperty &preferencesVector) const
{
	string movieName = "";
	double maxAngle = -1 * INFINITY, currAngle = INIT_VALUE;
	int index = 0;
	//Go through the movies the current user hasn't watch yet
	for (auto v : unitVector)
	{
		if (v.first == false) //check the current has ratted by this person
		{
			auto movieV = _getMovieInfoByIndex(index);
			if(movieV != _moviesMap.end())
			{
				currAngle = _calcAngleBtw(movieV->second, preferencesVector);
				if(currAngle >= maxAngle)
				{
					movieName = movieV->first;
					maxAngle = currAngle;
				}
			}
		}
		index++;
	}
	return movieName;
}
/**
* Sort help function to sort vector elements by second value
* @param p1 - first element
* @param p1 - second element
* @return true if the second element is bigger, false otherwise
*/
bool RecommenderSystem::_compBySecondProperty(const RecommenderSystem::RatingPair &p1,
											  const RecommenderSystem::RatingPair &p2) const
{
	return (p1.second < p2.second);
}
/**
* predict the user's score for the given user
* @param movieName - the movie to calc it's score
* @param userName - the person to recommend to
* @param k - positive int, the number of movies to take into account
* @return the recommended movie
*/
double RecommenderSystem::predictMovieScoreForUser(const string movieName,
												   const string userName,
		                                           const unsigned int k) const
{
	auto itPerson = _ratings.find(userName);
	//Check the person exist
	if(itPerson == _ratings.end())
	{
		return FAILURE; //User not found
	}
	//Check the movie exist
	auto movieToCalc = _getMovieInfo(movieName);
	if(movieToCalc == _moviesMap.end())
	{
		return FAILURE; //Movie not found
	}
	return _getMovieScore(movieToCalc, itPerson, k);
//	//Go through the movies the current user hasn't watch yet
//	int index = 0;
//	MovieRating ratingsVec(_numberOfMovies);
//	for (auto rating : itPerson->second)
//	{
//		if (rating.first == false) //check the current has ratted by this person
//		{
//			auto movieV = _getMovieInfoByIndex(index);
//			if(movieV != _moviesMap.end())
//			{
//				double currAngle =_calcAngleBtw(movieV->second, movieToCalc->second);
//				ratingsVec.push_back(make_pair(movieV->first, currAngle));
//			}
//		}
//		index++;
//	}
//	MovieSet kTopMovies = _getKMaxMovies(ratingsVec, k);
//
//	double score = _calcMovieScore(kTopMovies, itPerson);
//	return score;
}
/**
* Get the movie score according to the CF formula
* @param itMovie - the movie's info
* @param itPerson - the person's ratings
* @return the movie prediction score
*/
double RecommenderSystem::_getMovieScore(const RecommenderSystem::iteratorMovie &itMovie,
										 const RecommenderSystem::iteratorRating &itPerson,
										 const unsigned int k) const
{
	//Go through the movies the current user hasn't watch yet
	int index = 0;
	MovieRating ratingsVec;
	for (auto rating : itPerson->second)
	{
		if (rating.first == true) //check the current has ratted by this person
		{
			auto movieV = _getMovieInfoByIndex(index);
			if(movieV != _moviesMap.end())
			{
				double currAngle = _calcAngleBtw(movieV->second, itMovie->second);
				ratingsVec.push_back(make_pair(movieV->first, currAngle));
			}
		}
		index++;
	}
	MovieSet kTopMovies = _getKMaxMovies(ratingsVec, k);

	double score = _calcMovieScore(kTopMovies, itPerson);
	return score;
}

/**
* Gets the recommended movie for the given user, calc by CF
* @param userName - the person to recommend to
* @param k - positive int, the number of movies to take into account
* @return the recommended movie
*/
string RecommenderSystem::recommendByCF(const string& userName, const unsigned int k) const
{
	const string BLANK_MOVIE = "";
	string movieToReturn = "";
	auto itPerson = _ratings.find(userName);
	//Check the person exist
	if(itPerson == _ratings.end())
	{
		return ERROR_NO_USER; //User not found
	}
	//Go through the movies the current user hasn't watch yet
	int index = 0;
	MovieRating ratingsVec;
	for (auto rating : itPerson->second)
	{
		if (rating.first == false) //check the current hasn't ratted by this person
		{
			auto movieToCalc = _getMovieInfoByIndex(index);
			if(movieToCalc != _moviesMap.end())
			{
				double score = _getMovieScore(movieToCalc, itPerson, k);
				ratingsVec.push_back(make_pair(movieToCalc->first, score));
			}
		}
		index++;
	}
	double max = -1 * INFINITY;
	for (auto movie : ratingsVec)
	{
		if(movie.second > max && movie.first != BLANK_MOVIE)
		{
			movieToReturn = movie.first;
			max = movie.second;
		}
	}
	return movieToReturn;
}

/**
* Get the k most similar movies
* @param movieVector - the angle vector of movies
* @param k - positive int, the number of movies to take into account
* @return the vector of the k most similar movies
*/
RecommenderSystem::MovieSet RecommenderSystem::_getKMaxMovies(
		const RecommenderSystem::MovieRating &movieVector, const unsigned int k) const
{
	unsigned int count = 0;
	MovieSet maxMovies;
	for ( auto itMovie : movieVector )
	{
		if (maxMovies.empty())
		{
			maxMovies.insert(itMovie);
			count++;
			continue;
		}
		if (itMovie.second > maxMovies.begin()->second || count < k)
		{
			maxMovies.insert(itMovie);
			count++;
			while (count > k)
			{
				maxMovies.erase(maxMovies.begin());
				count--;
			}
		}
	}
	return maxMovies;
}
/**
* Calc the movie score according to the CF formula
* @param movieSet - set of the the k most similar movies
* @param itPerson - the person's ratings
* @return the movie prediction score
*/
double RecommenderSystem::_calcMovieScore(const RecommenderSystem::MovieSet &movieSet,
										  const iteratorRating &itPerson) const
{
	double denominator = INIT_VALUE;
	double numerator = INIT_VALUE;
	for (auto itMovie : movieSet)
	{
		double movieScore = _getMovieRating(itMovie.first, itPerson);
		numerator += (movieScore * itMovie.second);
		denominator += itMovie.second;
	}

	if(denominator != INIT_VALUE)
	{
		return numerator / denominator;
	}
	else
	{
		return INIT_VALUE;
	}
}

/**
* Get movie info by movie index in _movies
* @param movieName - movie to fetch it's rating
* @return the movie's rating
*/
double RecommenderSystem::_getMovieRating(const string movieName,
										  const RecommenderSystem::iteratorRating &itPerson) const
{
	auto itMovie = find(_movies.begin(), _movies.end(), movieName);
	if(itMovie != _movies.end())
	{
		int index = distance(_movies.begin(), itMovie);
		if(index >= 0)
		{
			auto ratingV = itPerson->second.at(index);
			if(ratingV.first == true)
			{
				return ratingV.second;
			}
		}
	}
	return INIT_VALUE;
}
/**
* Fills the _movies vector
* @param iss - data to insert
*/
void RecommenderSystem::_setMoviesArray(istringstream &iss)
{
	_movies = { istream_iterator<string>(iss), {}};
}
/**
* Fills the ratings values vector
* @param values - vector to change
* @param iss - data to insert
*/
void RecommenderSystem::_setVectorRatings(RecommenderSystem::VectorRatings &values,
										  istringstream &iss)
{
	int index = 0;
	string personName = "";
	//Split the string on space
	for(string s; iss >> s; )
	{
		if(index == 0) //get the person's name
		{
			personName = s;
		}
		else
		{
			if(s != NO_RATING)
			{
				//insert the property value to the vector
				values.push_back(make_pair(true, atof(s.c_str())));
			}
			else
			{
				//insert the property value to the vector
				values.push_back(make_pair(false, INIT_VALUE));
			}
		}
		index++;
	}
	values.shrink_to_fit();
	_ratings[personName] = values; //insert new element to the ratings' map
	_numberOfPeople++; //update movie counter
}




