#include "Presubmit.hpp"
#include "helpers.h"

int main() {
	return runPreSubmissionChecks() ? EXIT_SUCCESS : EXIT_FAILURE;
}