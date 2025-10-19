#include "lian.h"

LIAN::LIAN(Map& map, Cell* start, Cell* end, float stepLimit, float maxAngle)
    : map(map), start(start), end(end), stepLimit(stepLimit), maxAngle(maxAngle)
{
    if (!start || !end){
        cout << "Начальная или конечная точка не задана" << '\n';
        return;
    }
    if(stepLimit <= 0.0f)
        this->stepLimit = 1.0f;
}

float LIAN::calculateH(Cell* current, Cell* end) {
    return sqrt(float((current->x - end->x) * (current->x - end->x) + (current->y - end->y) * (current->y - end->y)));
}
vector<Cell*> LIAN::getNeighbours(Cell* current) {
    vector<Cell*> neighbours;
    int r = static_cast<int>(floor(stepLimit));
    int limit2 = stepLimit * stepLimit;
    float eps = 0.25f * limit2;

    for (int dy = -r; dy <= r; ++dy) {
        for (int dx = -r; dx<= r; ++dx){
            if (dx == 0 && dy == 0) continue;

            int nx = current->x + dx;
            int ny = current->y + dy;

            if (nx < 0 || ny < 0 || nx >= map.width || ny >= map.height) continue;

            float dist2 = float(dx*dx + dy*dy);
            if (fabs(dist2 - limit2) > eps) continue;

            Cell* cand = map.getCell(nx,ny);
            if (!cand || cand->obstacle) continue;

            float dist = sqrt(float(dx*dx + dy*dy));
            if (current->parent) {
                float vx1 = float(current->x - current->parent->x);
                float vy1 = float(current->y - current->parent->y);

                float vx2 = float(cand->x - current->x);
                float vy2 = float(cand->y - current->y);

                float len1 = sqrt(vx1*vx1 + vy1*vy1);
                float len2 = sqrt(vx2*vx2 + vy2*vy2);

                if (len1 > 0 && len2 > 0) {
                    float cosAngle = (vx1*vx2 + vy1*vy2) / (len1 * len2);
                    cosAngle = max(-1.0f, min(1.0f, cosAngle));
                    float angleDeg = acos(cosAngle) * 180.0f / M_PI;

                    if (angleDeg > maxAngle) {
                        continue;
                        //cand->angle = angleDeg;
                    }
                }
            }

            if (isLineOfSight(current, cand)) {
                neighbours.push_back(cand);
            }
        }
    }

    return neighbours;
}

bool LIAN::isLineOfSight(const Cell* from, const Cell* to) {
    int x0 = from->x;
    int y0 = from->y;

    int x1 = to->x;
    int y1 = to->y;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    int x = x0;
    int y = y0;

    while (true) {
        if(!map.isFree(x,y))
            return false;
        if (x == x1 && y == y1)
            break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }

    }

    return true;
}

vector<Cell*> LIAN::findPath() {
    vector<Cell*> path;
    
    while (!openList.empty()) 
        openList.pop();
    openSet.clear();
    closedSet.clear();


    start->g = 0.0f;
    start->h=calculateH(start, end);
    start->f = start->g + start->h;
    start->parent = nullptr;
    
    openList.push(start);
    openSet.insert(start);
    while(!openList.empty()) {
        Cell* current = openList.top();
        openList.pop();

        while (closedSet.find(current) != closedSet.end() && !openList.empty()) {
            current = openList.top();
            openList.pop();
        }

        if(*current == *end) {
            Cell* it = current;
            while (it) {
                path.push_back(it);
                it = it->parent;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        openSet.erase(current);
        closedSet.insert(current);

        vector<Cell*> neighbours = getNeighbours(current);
        for (Cell* neighbour : neighbours) {
            if (closedSet.find(neighbour) != closedSet.end())
                continue; 

            float tentativeG = current->g + current->distanceTo(*neighbour);
            bool inOpen = (openSet.find(neighbour) != openSet.end());
            if (!inOpen || tentativeG < neighbour->g) {
                neighbour->parent = current;
                neighbour->g = tentativeG;
                neighbour->h = calculateH(neighbour, end);
                neighbour->f = neighbour->g + neighbour->h;

                if (!inOpen) {
                    openList.push(neighbour);
                    openSet.insert(neighbour);
                }
            }
        }
    }

    return path;
}
