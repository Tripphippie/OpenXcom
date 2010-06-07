/*
 * Copyright 2010 Daniel Albano
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "BaseView.h"

/**
 * Sets up a base view with the specified size and position.
 * @param width Width in pixels.
 * @param height Height in pixels.
 * @param x X position in pixels.
 * @param y Y position in pixels.
 */
BaseView::BaseView(int width, int height, int x, int y) : InteractiveSurface(width, height, x, y), _base(0), _texture(0), _selFacility(0)
{
	for (int x = 0; x < BASE_SIZE; x++)
		for (int y = 0; y < BASE_SIZE; y++)
			_facilities[x][y] = 0;
}

/**
 *
 */
BaseView::~BaseView()
{
}

void BaseView::setBase(Base *base)
{
	_base = base;

	for (int x = 0; x < BASE_SIZE; x++)
		for (int y = 0; y < BASE_SIZE; y++)
			_facilities[x][y] = 0;

	for (vector<BaseFacility*>::iterator i = _base->getFacilities()->begin(); i != _base->getFacilities()->end(); i++)
	{
		for (int y = (*i)->getY(); y < (*i)->getY() + (*i)->getRules()->getSize(); y++)
		{
			for (int x = (*i)->getX(); x < (*i)->getX() + (*i)->getRules()->getSize(); x++)
			{				
				_facilities[x][y] = *i;
			}
		}
	}

	draw();
}

void BaseView::setTexture(SurfaceSet *texture)
{
	_texture = texture;
}

void BaseView::draw()
{
	// Draw grid squares
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			_texture->getSurface()->setX(x * GRID_SIZE);
			_texture->getSurface()->setY(y * GRID_SIZE);
			_texture->getFrame(0)->blit(this);
		}
	}

	vector<Craft*>::iterator craft = _base->getCrafts()->begin();
	
	for (vector<BaseFacility*>::iterator i = _base->getFacilities()->begin(); i != _base->getFacilities()->end(); i++)
	{
		// Draw facility shape
		int num = 0;
		for (int y = (*i)->getY(); y < (*i)->getY() + (*i)->getRules()->getSize(); y++)
		{
			for (int x = (*i)->getX(); x < (*i)->getX() + (*i)->getRules()->getSize(); x++)
			{				
				_texture->getSurface()->setX(x * GRID_SIZE);
				_texture->getSurface()->setY(y * GRID_SIZE);

				if ((*i)->getBuildTime() == 0)
					_texture->getFrame((*i)->getRules()->getSpriteShape() + num)->blit(this);
				else
					_texture->getFrame((*i)->getRules()->getSpriteShape() + num + 2 + (*i)->getRules()->getSize())->blit(this);

				num++;
			}
		}

		// Draw connectors
		if ((*i)->getBuildTime() == 0)
		{
			// Facilities to the right
			int x = (*i)->getX() + (*i)->getRules()->getSize();
			if (x < BASE_SIZE)
			{
				for (int y = (*i)->getY(); y < (*i)->getY() + (*i)->getRules()->getSize(); y++)
				{
					if (_facilities[x][y] != 0 && _facilities[x][y]->getBuildTime() == 0)
					{
						_texture->getSurface()->setX(x * GRID_SIZE - GRID_SIZE / 2);
						_texture->getSurface()->setY(y * GRID_SIZE);
						_texture->getFrame(7)->blit(this);
					}
				}
			}

			// Facilities to the bottom
			int y = (*i)->getY() + (*i)->getRules()->getSize();
			if (y < BASE_SIZE)
			{
				for (int x = (*i)->getX(); x < (*i)->getX() + (*i)->getRules()->getSize(); x++)
				{
					if (_facilities[x][y] != 0 && _facilities[x][y]->getBuildTime() == 0)
					{
						_texture->getSurface()->setX(x * GRID_SIZE);
						_texture->getSurface()->setY(y * GRID_SIZE - GRID_SIZE / 2);
						_texture->getFrame(8)->blit(this);
					}
				}
			}
		}

		// Draw facility graphic
		num = 0;
		for (int y = (*i)->getY(); y < (*i)->getY() + (*i)->getRules()->getSize(); y++)
		{
			for (int x = (*i)->getX(); x < (*i)->getX() + (*i)->getRules()->getSize(); x++)
			{				
				_texture->getSurface()->setX(x * GRID_SIZE);
				_texture->getSurface()->setY(y * GRID_SIZE);

				if ((*i)->getRules()->getSize() == 1)
					_texture->getFrame((*i)->getRules()->getSpriteFacility() + num)->blit(this);

				num++;
			}
		}

		// Draw crafts
		if ((*i)->getRules()->getCrafts() > 0 && craft != _base->getCrafts()->end())
		{
			_texture->getSurface()->setX((*i)->getX() * GRID_SIZE + ((*i)->getRules()->getSize() - 1) * GRID_SIZE / 2);
			_texture->getSurface()->setY((*i)->getY() * GRID_SIZE + ((*i)->getRules()->getSize() - 1) * GRID_SIZE / 2);

			_texture->getFrame((*craft)->getRules()->getSprite())->blit(this);
			craft++;
		}
	}
}

BaseFacility *BaseView::getSelectedFacility()
{
	return _selFacility;
}

void BaseView::handle(SDL_Event *ev, int scale, State *state)
{
	if (ev->type == SDL_MOUSEMOTION)
	{
		double x = ev->button.x - _x * scale;
		double y = ev->button.y - _y * scale;
		int xx = (int)floor(x / (GRID_SIZE * scale));
		int yy = (int)floor(y / (GRID_SIZE * scale));
		if (xx >= 0 && xx < BASE_SIZE && yy >= 0 && yy < BASE_SIZE)
			_selFacility = _facilities[xx][yy];
		else
			_selFacility = 0;
	}
}