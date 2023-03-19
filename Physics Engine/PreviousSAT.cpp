void RECTANGLE_RECTANGLE(shapes::Rectangle& rec1, shapes::Rectangle& rec2) {
	// Calculate the four vertices of each rectangle
	math::Vector2<float>* vertices1 = rec1.getVertices();
	math::Vector2<float>* vertices2 = rec2.getVertices();

	math::Vector2<float> edgeNormals[4];
	for (int index = 0; index < 8; index++) {
		int otherIndex = index + 1;
		if (otherIndex == 4) otherIndex = 0;

		auto& vertex = vertices1[index];
		auto& otherVertex = vertices1[otherIndex];
		math::Vector2 parallelVector{ otherVertex.x - vertex.x, otherVertex.y - vertex.y };
		math::Vector2 perpVector{ -parallelVector.y, parallelVector.x };
		float magnitude = std::sqrt(std::pow(perpVector.x, 2) + std::pow(perpVector.y, 2));
		math::Vector2 normalVector{ perpVector.x / magnitude, perpVector.y / magnitude };
		edgeNormals[index] = normalVector;
	}
	for (int index = 0; index < 4; index++) {
		int otherIndex = index + 1;
		if (otherIndex == 4) otherIndex = 0;

		auto& vertex = vertices2[index];
		auto& otherVertex = vertices2[otherIndex];
		math::Vector2 parallelVector{ otherVertex.x - vertex.x, otherVertex.y - vertex.y };
		math::Vector2 perpVector{ -parallelVector.y, parallelVector.x };
		float magnitude = std::sqrt(std::pow(perpVector.x, 2) + std::pow(perpVector.y, 2));
		math::Vector2 normalVector{ perpVector.x / magnitude, perpVector.y / magnitude };
		edgeNormals[index + 4] = normalVector;
	}

	// Check for overlap on each axis
	float smallestOverlap = INFINITY;
	math::Vector2<float> smallestAxis;
	for (auto& axis : edgeNormals) {
		// Project the vertices of both rectangles onto the axis
		float min1 = INFINITY, max1 = -INFINITY, min2 = INFINITY, max2 = -INFINITY;
		for (int index = 0; index < 4; index++) {
			auto& vertex = vertices1[index];
			float projection = vertex.dot(axis);

			min1 = std::min(min1, projection);
			max1 = std::max(max1, projection);
		}
		for (int index = 0; index < 4; index++) {
			auto& vertex = vertices2[index];
			float projection = vertex.dot(axis);
			min2 = std::min(min2, projection);
			max2 = std::max(max2, projection);
		}

		// Check for overlap
		if (max1 < min2 || max2 < min1) {
			// No overlap, so there is no collision
			delete[] vertices1;
			delete[] vertices2;
			return;
		}

		float overlap = max1 > max2 ? -(max2 - min1) : max1 - min2;

		if (abs(overlap) < smallestOverlap) {
			smallestOverlap = abs(overlap);
			smallestAxis = axis * overlap / abs(overlap);
		}
	}

	// If we get here, there is a collision
	std::cout << "collision";
	smallestAxis.print();
	std::cin.get();
	delete[] vertices1;
	delete[] vertices2;
}